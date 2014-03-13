#include "wire.h"
#include "wire_fd.h"
#include "wire_pool.h"
#include "wire_stack.h"
#include "macros.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include "utils.h"

static wire_thread_t wire_main;
static wire_t task_accept;
static wire_pool_t echo_pool;

static void task_echo_run(void *arg)
{
	int fd = (long int)arg;
	int ret;
	wire_fd_state_t fd_state;

	wire_fd_mode_init(&fd_state, fd);
	wire_fd_mode_read(&fd_state);

	set_nonblock(fd);

	do {
		wire_fd_wait(&fd_state);

		char buf[1024];
		ret = read(fd, buf, sizeof(buf));
		if (ret == 0) {
			break;
		} else if (ret < 0) {
			if (errno == EINTR || errno == EAGAIN)
				continue;
			else
				break;
		}

		ret = write(fd, buf, ret);
		if (ret < 0) {
			if (errno == EINTR || errno == EAGAIN)
				continue;
			else
				break;
		}
	} while (ret >= 0);

	wire_fd_mode_none(&fd_state);
	close(fd);
	printf("echo is done\n");
}

static void task_accept_run(void *arg)
{
	UNUSED(arg);
	int fd = socket_setup(9876);
	if (fd < 0)
		return;

	wire_fd_state_t fd_state;
	wire_fd_mode_init(&fd_state, fd);
	wire_fd_mode_read(&fd_state);

	while (1) {
		wire_fd_wait(&fd_state);
		int new_fd = accept(fd, NULL, NULL);
		if (new_fd >= 0) {
			printf("New connection: %d\n", new_fd);
			wire_t *task = wire_pool_alloc(&echo_pool, "echo", task_echo_run, (void*)(long int)new_fd);
			if (!task) {
				printf("Echo is busy, sorry\n");
				close(new_fd);
			}
		} else {
			if (errno != EINTR && errno != EAGAIN) {
				perror("Error accepting from listening socket");
				break;
			}
		}
	}
}

int main()
{
	wire_thread_init(&wire_main);
	wire_fd_init();
	wire_pool_init(&echo_pool, NULL, 6, 4096);
	wire_init(&task_accept, "accept", task_accept_run, NULL, WIRE_STACK_ALLOC(4096));
	wire_thread_run();
	return 0;
}
