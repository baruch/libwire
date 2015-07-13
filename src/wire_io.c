#include "wire_io.h"

#include "wire.h"
#include "wire_fd.h"
#include "wire_stack.h"

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>

struct wire_io {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	struct list_head list;
	wire_fd_state_t fd_state;
	int response_send_fd;
	int response_recv_fd;
	int num_active_ios;
	wire_t wire;
} wire_io;

struct wire_io_act_common {
	struct list_head elem;
	wire_wait_t *wait;
};

static void wakeup_fd_listener(void)
{
	if (wire_io.num_active_ios == 0)
		wire_fd_mode_read(&wire_io.fd_state);
	wire_io.num_active_ios++;
}

/* This runs in the wire thread and should hopefully only see rare lock contention and as such not really block at all.
 */
static void submit_action(struct wire_io *wio, struct wire_io_act_common *act)
{
	wire_wait_list_t wait_list;
	wire_wait_t wait_item;

	wire_wait_list_init(&wait_list);
	wire_wait_init(&wait_item);
	wire_wait_chain(&wait_list, &wait_item);

	act->wait = &wait_item;

	// Add the action to the list
	pthread_mutex_lock(&wio->mutex);
	list_add_tail(&act->elem, &wio->list);
	pthread_mutex_unlock(&wio->mutex);

	// Wake at least one worker thread to get this action done
	pthread_cond_signal(&wio->cond);

	// Wait for the action to complete
	wakeup_fd_listener();
	wire_list_wait(&wait_list);
}

static int read_file_content(const char *filename, char *buf, size_t bufsz)
{
	int fd;
	int ret;
	int save_errno;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return -1;

	ret = read(fd, buf, bufsz);
	save_errno = errno;

	close(fd);

	errno = save_errno;
	return ret;
}

static pid_t spawn(char * *const args, int *stdin_fd, int *stdout_fd, int *stderr_fd)
{
	pid_t pid;
	int ret;
	int inpipe[2] = {-1, -1};
	int outpipe[2] = {-1, -1};
	int errpipe[2] = {-1, -1};
	int errno_save;

	if (stdin_fd) {
		ret = socketpair(AF_UNIX, SOCK_STREAM, 0, inpipe);
		if (ret < 0)
			goto pre_exec_failure;
		*stdin_fd = inpipe[1];
	}

	if (stdout_fd) {
		ret = socketpair(AF_UNIX, SOCK_STREAM, 0, outpipe);
		if (ret < 0)
			goto pre_exec_failure;
		*stdout_fd = outpipe[0];
	}

	if (stderr_fd) {
		ret = socketpair(AF_UNIX, SOCK_STREAM, 0, errpipe);
		if (ret < 0)
			goto pre_exec_failure;
		*stderr_fd = errpipe[0];
	}

	pid = fork();

	// Still parent
	if (pid < 0) {
		goto pre_exec_failure;
	}
	if (pid > 0) {
		if (stdin_fd)
			close(inpipe[0]);
		if (stdout_fd)
			close(outpipe[1]);
		if (stderr_fd)
			close(errpipe[1]);
		return pid;
	}

	// Child, must never return
	if (stdin_fd) {
		close(inpipe[1]);
		dup2(inpipe[0], 0);
		close(inpipe[0]);
	}
	if (stdout_fd) {
		close(outpipe[0]);
		dup2(outpipe[1], 1);
		close(outpipe[1]);
	}
	if (stderr_fd) {
		close(errpipe[0]);
		dup2(errpipe[1], 2);
		close(errpipe[1]);
	}
	execv(args[0], args);
	_exit(errno);

pre_exec_failure:
	errno_save = errno;
	if (stdin_fd) {
		close(inpipe[0]);
		close(inpipe[1]);
	}
	if (stdout_fd) {
		close(outpipe[0]);
		close(outpipe[1]);
	}
	if (stderr_fd) {
		close(errpipe[0]);
		close(errpipe[1]);
	}
	errno = errno_save;
	return -1;
}

#include "wire_io_gen.c.inc"

static inline void set_nonblock(int fd)
{
        int ret = fcntl(fd, F_GETFL);
        if (ret < 0)
                return;

        fcntl(fd, F_SETFL, ret | O_NONBLOCK);
}

/* Return the performed action back to the caller */
static void return_action(struct wire_io *wio, struct wire_io_act *act)
{
	ssize_t ret = write(wio->response_send_fd, &act, sizeof(act));
	if (ret != sizeof(act))
		printf("wire_io: returning action failed in write, ret=%d errno=%d:  %m\n", (int)ret, errno);
}

/* Wait with an unlocked mutex on the condition until we are woken up, when we
 * are woken up the mutex is retaken and we can manipulate the list as we wish
 * and must ensure to unlock it and do it as fast as possible to reduce
 * contention.
 */
static struct wire_io_act *get_action(struct wire_io *wio)
{
	pthread_mutex_lock(&wio->mutex);

	while (list_empty(&wio->list))
		pthread_cond_wait(&wio->cond, &wio->mutex);

	struct list_head *head = list_head(&wio->list);
	struct wire_io_act *entry = NULL;
	if (head) {
		list_del(head);
		entry = (struct wire_io_act*)list_entry(head, struct wire_io_act_common, elem);
	}

	pthread_mutex_unlock(&wio->mutex);

	return entry;
}

static void block_signals(void)
{
	sigset_t sig_set;

	sigfillset(&sig_set);
	pthread_sigmask(SIG_BLOCK, &sig_set, NULL);
}

/* The async thread implementation that waits for async actions to perform and runs them.
 */
static void *wire_io_thread(void *arg)
{
	struct wire_io *wio = arg;

	block_signals();

	while (1) {
		struct wire_io_act *act = get_action(wio);
		if (!act) {
			continue;
		}

		perform_action(act);
		return_action(wio, act);
	}
	return NULL;
}

/* Take care of the response from the worker threads, gets the action that was
 * performed and resumes the caller to take care of the response.
 */
static void wire_io_response(void *arg)
{
	struct wire_io *wio = arg;

	set_nonblock(wio->response_recv_fd);

	while (1) {
		struct wire_io_act *act[32];
		ssize_t ret = read(wio->response_recv_fd, act, sizeof(act));
		if (ret > 0) {
			unsigned i;
			const unsigned num_ret = ret / sizeof(act[0]);
			for (i = 0; i < num_ret; i++) {
				wire_wait_resume(act[i]->common.wait);
				wio->num_active_ios--;
				if (wio->num_active_ios == 0)
					wire_fd_mode_none(&wio->fd_state);
			}
		} else if (ret < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				// The fd state is set to read by the submitter in SEND_RET macro
				wire_wait_reset(&wio->fd_state.wait);
				wire_fd_wait(&wio->fd_state); // Wait for the response, only if we would block
			} else {
				fprintf(stderr, "Error reading from socket for wire_io: %d = %m\n", errno);
				abort();
			}
		} else {
			fprintf(stderr, "EOF on the socketpair is highly improbable\n");
			abort();
		}
	}

	wire_fd_mode_none(&wio->fd_state);
	close(wio->response_recv_fd);
	close(wio->response_send_fd);
	pthread_cond_destroy(&wio->cond);
	pthread_mutex_destroy(&wio->mutex);
}

void wire_io_init(int num_threads)
{
	wire_io.num_active_ios = 0;
	list_head_init(&wire_io.list);
	pthread_mutex_init(&wire_io.mutex, NULL);
	pthread_cond_init(&wire_io.cond, NULL);

	int sfd[2];
	int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, sfd);
	if (ret < 0) {
		fprintf(stderr, "Error initializing a socketpair for wire_io: %m\n");
		abort();
	}

	wire_io.response_send_fd = sfd[0];
	wire_io.response_recv_fd = sfd[1];

	wire_fd_mode_init(&wire_io.fd_state, wire_io.response_recv_fd);
	wire_init(&wire_io.wire, "wire_io", wire_io_response, &wire_io, WIRE_STACK_ALLOC(4096));

	int i;
	for (i = 0; i < num_threads; i++) {
		pthread_t th;
		pthread_create(&th, NULL, wire_io_thread, &wire_io);
	}
}
