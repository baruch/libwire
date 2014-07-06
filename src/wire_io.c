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

enum wio_type {
	IO_OPEN,
	IO_CLOSE,
	IO_PREAD,
	IO_PWRITE,
	IO_READ,
	IO_WRITE,
	IO_FALLOCATE,
	IO_FTRUNCATE,
	IO_FSYNC,
	IO_FSTAT,
	IO_STAT,
	IO_STATFS,
	IO_FSTATFS,
	IO_GETADDRINFO,
};

struct wire_io_act {
	struct list_head elem;
	enum wio_type type;
	union {
		struct {
			const char *pathname;
			int flags;
			mode_t mode;
			int ret;
			int verrno;
		} open;
		struct {
			int fd;
			int ret;
			int verrno;
		} close;
		struct {
			int fd;
			void *buf;
			size_t count;
			off_t offset;
			int ret;
			int verrno;
		} pread;
		struct {
			int fd;
			const void *buf;
			size_t count;
			off_t offset;
			int ret;
			int verrno;
		} pwrite;
		struct {
			int fd;
			void *buf;
			size_t count;
			int ret;
			int verrno;
		} read;
		struct {
			int fd;
			const void *buf;
			size_t count;
			int ret;
			int verrno;
		} write;
		struct {
			int fd;
			struct stat *buf;
			int ret;
			int verrno;
		} fstat;
		struct {
			const char *path;
			struct stat *buf;
			int ret;
			int verrno;
		} stat;
		struct {
			int fd;
			off_t length;
			int ret;
			int verrno;
		} ftruncate;
		struct {
			int fd;
			int mode;
			off_t offset;
			off_t len;
			int ret;
			int verrno;
		} fallocate;
		struct {
			int fd;
			int ret;
			int verrno;
		} fsync;
		struct {
			const char *path;
			struct statfs *buf;
			int ret;
			int verrno;
		} statfs;
		struct {
			int fd;
			struct statfs *buf;
			int ret;
			int verrno;
		} fstatfs;
		struct {
			const char *node;
			const char *service;
			const struct addrinfo *hints;
			struct addrinfo **res;
			int ret;
			int verrno;
		} getaddrinfo;
	};
	wire_wait_t *wait;
};

static inline void set_nonblock(int fd)
{
        int ret = fcntl(fd, F_GETFL);
        if (ret < 0)
                return;

        fcntl(fd, F_SETFL, ret | O_NONBLOCK);
}

/* This runs in the wire thread and should hopefully only see rare lock contention and as such not really block at all.
 */
static void submit_action(struct wire_io *wio, struct wire_io_act *act)
{
	wire_wait_list_t wait_list;
	wire_wait_t wait;

	wire_wait_list_init(&wait_list);
	wire_wait_init(&wait);
	wire_wait_chain(&wait_list, &wait);

	act->wait = &wait;

	// Add the action to the list
	pthread_mutex_lock(&wio->mutex);
	list_add_tail(&act->elem, &wio->list);
	pthread_mutex_unlock(&wio->mutex);

	// Wake at least one worker thread to get this action done
	pthread_cond_signal(&wio->cond);

	// Wait for the action to complete
	wire_list_wait(&wait_list);
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
		entry = list_entry(head, struct wire_io_act, elem);
	}

	pthread_mutex_unlock(&wio->mutex);

	return entry;
}

/* Perform the actions, add here any blocking operation needed */
static void perform_action(struct wire_io_act *act)
{
#define RUN_RET(_name_, run) act->_name_.ret = run; act->_name_.verrno = errno
	//DEBUG: printf("performing action %p\n", act);
	switch (act->type) {
		case IO_OPEN:
			RUN_RET(open, open(act->open.pathname, act->open.flags, act->open.mode));
			break;
		case IO_CLOSE:
			RUN_RET(close, close(act->close.fd));
			break;
		case IO_PREAD:
			RUN_RET(pread, pread(act->pread.fd, act->pread.buf, act->pread.count, act->pread.offset));
			break;
		case IO_PWRITE:
			RUN_RET(pwrite, pwrite(act->pwrite.fd, act->pwrite.buf, act->pwrite.count, act->pwrite.offset));
			break;
		case IO_READ:
			RUN_RET(read, read(act->read.fd, act->read.buf, act->read.count));
			break;
		case IO_WRITE:
			RUN_RET(write, write(act->write.fd, act->write.buf, act->write.count));
			break;
		case IO_FSTAT:
			RUN_RET(fstat, fstat(act->fstat.fd, act->fstat.buf));
			break;
		case IO_STAT:
			RUN_RET(stat, stat(act->stat.path, act->stat.buf));
			break;
		case IO_FTRUNCATE:
			RUN_RET(ftruncate, ftruncate(act->ftruncate.fd, act->ftruncate.length));
			break;
		case IO_FALLOCATE:
			RUN_RET(fallocate, fallocate(act->fallocate.fd, act->fallocate.mode, act->fallocate.offset, act->fallocate.len));
			break;
		case IO_FSYNC:
			RUN_RET(fsync, fsync(act->fsync.fd));
			break;
		case IO_STATFS:
			RUN_RET(statfs, statfs(act->statfs.path, act->statfs.buf));
			break;
		case IO_FSTATFS:
			RUN_RET(fstatfs, fstatfs(act->fstatfs.fd, act->fstatfs.buf));
			break;
		case IO_GETADDRINFO:
			RUN_RET(getaddrinfo, getaddrinfo(act->getaddrinfo.node, act->getaddrinfo.service, act->getaddrinfo.hints, act->getaddrinfo.res));
			break;
	}
	//DEBUG: printf("Done performing act %p\n", act);
#undef RUN_RET
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
				wire_wait_resume(act[i]->wait);
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

static void wakeup_fd_listener(void)
{
	if (wire_io.num_active_ios == 0)
		wire_fd_mode_read(&wire_io.fd_state);
	wire_io.num_active_ios++;
}

#define DEF(_type_) struct wire_io_act act; act.type = _type_
#define SEND_RET(_name_) \
	wakeup_fd_listener(); \
	submit_action(&wire_io, &act); \
	if (act._name_.ret < 0) \
		errno = act._name_.verrno; \
	return act._name_.ret

int wio_open(const char *pathname, int flags, mode_t mode)
{
	// Fill request data
	DEF(IO_OPEN);
	act.open.pathname = pathname;
	act.open.flags = flags;
	act.open.mode = mode;
	SEND_RET(open);
}

int wio_close(int fd)
{
	DEF(IO_CLOSE);
	act.close.fd = fd;
	SEND_RET(close);
}

ssize_t wio_pread(int fd, void *buf, size_t count, off_t offset)
{
	DEF(IO_PREAD);
	act.pread.fd = fd;
	act.pread.buf = buf;
	act.pread.count = count;
	act.pread.offset = offset;
	SEND_RET(pread);
}

ssize_t wio_pwrite(int fd, const void *buf, size_t count, off_t offset)
{
	DEF(IO_PWRITE);
	act.pwrite.fd = fd;
	act.pwrite.buf = buf;
	act.pwrite.count = count;
	act.pwrite.offset = offset;
	SEND_RET(pwrite);
}

ssize_t wio_read(int fd, void *buf, size_t count)
{
	DEF(IO_READ);
	act.read.fd = fd;
	act.read.buf = buf;
	act.read.count = count;
	SEND_RET(read);
}

ssize_t wio_write(int fd, const void *buf, size_t count)
{
	DEF(IO_WRITE);
	act.write.fd = fd;
	act.write.buf = buf;
	act.write.count = count;
	SEND_RET(write);
}

int wio_fstat(int fd, struct stat *buf)
{
	DEF(IO_FSTAT);
	act.fstat.fd = fd;
	act.fstat.buf = buf;
	SEND_RET(fstat);
}

int wio_stat(const char *path, struct stat *buf)
{
	DEF(IO_STAT);
	act.stat.path = path;
	act.stat.buf = buf;
	SEND_RET(stat);
}

int wio_ftruncate(int fd, off_t length)
{
	DEF(IO_FTRUNCATE);
	act.ftruncate.fd = fd;
	act.ftruncate.length = length;
	SEND_RET(ftruncate);
}

int wio_fallocate(int fd, int mode, off_t offset, off_t len)
{
	DEF(IO_FALLOCATE);
	act.fallocate.fd = fd;
	act.fallocate.mode = mode;
	act.fallocate.offset = offset;
	act.fallocate.len = len;
	SEND_RET(fallocate);
}

int wio_fsync(int fd)
{
	DEF(IO_FSYNC);
	act.fsync.fd = fd;
	SEND_RET(fsync);
}

int wio_statfs(const char *path, struct statfs *buf)
{
	DEF(IO_STATFS);
	act.statfs.path = path;
	act.statfs.buf = buf;
	SEND_RET(statfs);
}

int wio_fstatfs(int fd, struct statfs *buf)
{
	DEF(IO_FSTATFS);
	act.fstatfs.fd = fd;
	act.fstatfs.buf = buf;
	SEND_RET(fstatfs);
}

int wio_getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res)
{
	DEF(IO_GETADDRINFO);
	act.getaddrinfo.node = node;
	act.getaddrinfo.service = service;
	act.getaddrinfo.hints = hints;
	act.getaddrinfo.res = res;
	SEND_RET(getaddrinfo);
}
