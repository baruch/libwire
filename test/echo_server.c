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

static xcoro_t xcoro_main;
static xcoro_task_t task_accept;
static xcoro_task_pool_t echo_pool;

void set_nonblock(int fd)
{
	int ret = fcntl(fd, F_GETFL);
	if (ret < 0)
		return;

	fcntl(fd, F_SETFL, ret | O_NONBLOCK);
}

void set_reuse(int fd)
{
	int so_reuseaddr = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr, sizeof(so_reuseaddr));
}

int socket_setup(unsigned short port)
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 1) {
		perror("Failed to create socket");
		return -1;
	}

	set_nonblock(fd);
	set_reuse(fd);

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
	if (ret < 0) {
		perror("Failed to bind to socket");
		close(fd);
		return -1;
	}

	ret = listen(fd, 100);
	if (ret < 0) {
		perror("failed to listen to port");
		close(fd);
		return -1;
	}

	return fd;
}

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
