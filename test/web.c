#include "xcoro.h"
#include "xcoro_fd.h"
#include "xcoro_task_pool.h"
#include "xcoro_stack.h"
#include "macros.h"
#include "ebb_request_parser.h"

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
static xcoro_task_pool_t web_pool;

static void set_nonblock(int fd)
{
	int ret = fcntl(fd, F_GETFL);
	if (ret < 0)
		return;

	fcntl(fd, F_SETFL, ret | O_NONBLOCK);
}

static void set_reuse(int fd)
{
	int so_reuseaddr = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr, sizeof(so_reuseaddr));
}

static int socket_setup(unsigned short port)
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


static void on_path(ebb_request* request, const char *at, size_t length)
{
	int i;
	
	printf("Path: ");
	for (i = 0; i < length; i++)
		printf("%c", at[i]);
	printf("\n");
}

static void on_complete(ebb_request *request)
{
	printf("Complete\n");
}

static void on_headers_complete(ebb_request *request)
{
	printf("Headers complete\n");
}

static ebb_request *new_request(void *arg)
{
	ebb_request *request = arg;
	ebb_request_init(request);
	request->on_complete = on_complete;
	request->on_headers_complete = on_headers_complete;
	request->on_path = on_path;
	return request;
}

void task_web_run(void *arg)
{
	int fd = (long int)arg;
	int ret;
	xcoro_fd_state_t fd_state;
	ebb_request_parser parser;
	ebb_request request;

	xcoro_fd_mode_init(&fd_state, fd);
	xcoro_fd_mode_read(&fd_state);

	set_nonblock(fd);

	ebb_request_parser_init(&parser);
	parser.new_request = new_request;
	parser.data = &request;

	char buf[4096];
	size_t offset = 0;
	do {
		xcoro_fd_wait(&fd_state);

		ret = read(fd, buf+offset, sizeof(buf) - offset);
		if (ret == 0) {
			break;
		} else if (ret < 0) {
			if (errno == EINTR || errno == EAGAIN)
				continue;
			else
				break;
		}

		size_t processed = ebb_request_parser_execute(&parser, buf, offset+ret, 0);
		size_t left = offset+ret - processed;
		
		if (left > 0)
			memmove(buf + processed, buf, left);
		offset = left;
	} while (!ebb_request_parser_is_finished(&parser));

	snprintf(buf, sizeof(buf), "HTTP/1.1 200 OK\r\n\r\nTest\n");
	int sent = 0;
	do {
		ret = write(fd, buf + sent, strlen(buf) - sent);
		if (ret == 0)
			break;
		else if (ret > 0) {
			sent += ret;
			if (sent == strlen(buf))
				break;
		} else {
			// Error
			if (errno != EINTR && errno != EAGAIN)
				break;
		}

		xcoro_fd_mode_write(&fd_state);
		xcoro_fd_wait(&fd_state);
	} while (1);

	xcoro_fd_mode_none(&fd_state);
	close(fd);
}

void task_accept_run(void *arg)
{
	int fd = socket_setup(9090);
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
			char name[32];
			snprintf(name, sizeof(name), "web %d", new_fd);
			xcoro_task_t *task = xcoro_task_pool_alloc(&web_pool, name, task_web_run, (void*)(long int)new_fd);
			if (!task) {
				printf("Web server is busy, sorry\n");
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
	xcoro_task_pool_init(&web_pool, NULL, 16, 16*1024);
	xcoro_task_init(&task_accept, "accept", task_accept_run, NULL, xcoro_stack_alloc(4096), 4096);
	xcoro_run();
	return 0;
}
