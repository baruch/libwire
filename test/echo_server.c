#include "xcoro.h"
#include "xcoro_fd.h"
#include "xcoro_task_pool.h"
#include "xcoro_stack.h"
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

static xcoro_t xcoro_main;
static xcoro_task_t task_accept;
static xcoro_task_pool_t echo_pool;

void task_echo_run(void *arg)
{
	int fd = (long int)arg;
	int ret;
	xcoro_fd_state_t fd_state;

	xcoro_fd_mode_init(&fd_state, fd);
	xcoro_fd_mode_read(&fd_state);

	set_nonblock(fd);

	do {
		xcoro_fd_wait(&fd_state);

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

	xcoro_fd_mode_none(&fd_state);
	close(fd);
	printf("echo is done\n");
}

void task_accept_run(void *arg)
{
	UNUSED(arg);
	int fd = socket_setup(9876);
	if (fd < 0)
		return;

	xcoro_fd_state_t fd_state;
	xcoro_fd_mode_init(&fd_state, fd);
	xcoro_fd_mode_read(&fd_state);

	while (1) {
		xcoro_fd_wait(&fd_state);
		int new_fd = accept(fd, NULL, NULL);
		if (new_fd >= 0) {
			printf("New connection: %d\n", new_fd);
			xcoro_task_t *task = xcoro_task_pool_alloc(&echo_pool, "echo", task_echo_run, (void*)(long int)new_fd);
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
	xcoro_init(&xcoro_main);
	xcoro_fd_init();
	xcoro_task_pool_init(&echo_pool, NULL, 6, 4096);
	xcoro_task_init(&task_accept, "accept", task_accept_run, NULL, XCORO_STACK_ALLOC(4096));
	xcoro_run();
	return 0;
}
