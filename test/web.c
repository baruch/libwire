#include "wire.h"
#include "wire_fd.h"
#include "wire_pool.h"
#include "wire_stack.h"
#include "macros.h"
#include "http_parser.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdarg.h>

#include "utils.h"

#ifdef NDEBUG
#define DEBUG(fmt, ...)
#else
#define DEBUG(fmt, ...) xlog(fmt, ## __VA_ARGS__)
#endif

#define WEB_POOL_SIZE 128

static wire_thread_t wire_thread_main;
static wire_t wire_accept;
static wire_pool_t web_pool;

struct web_data {
	int fd;
	wire_fd_state_t fd_state;
};

static void xlog(const char *fmt, ...)
{
	char msg[256];
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(msg, sizeof(msg), fmt, ap);
	va_end(ap);

	puts(msg);
}

static int on_message_begin(http_parser *parser)
{
	UNUSED(parser);
	DEBUG("Message begin");
	return 0;
}

static int on_headers_complete(http_parser *parser)
{
	UNUSED(parser); // When in NDEBUG
	DEBUG("Headers complete: HTTP/%d.%d %s", parser->http_major, parser->http_minor, http_method_str(parser->method));
	return 0;
}

static int buf_write(wire_fd_state_t *fd_state, const char *buf, int len)
{
	int sent = 0;
	do {
		int ret = write(fd_state->fd, buf + sent, len - sent);
		if (ret == 0)
			return -1;
		else if (ret > 0) {
			sent += ret;
			if (sent == len)
				return 0;
		} else {
			// Error
			if (errno != EINTR && errno != EAGAIN)
				return -1;
		}

		wire_fd_mode_write(fd_state);
		wire_fd_wait(fd_state);
	} while (1);
}

static int on_message_complete(http_parser *parser)
{
	DEBUG("message complete");
	struct web_data *d = parser->data;
	char buf[512];
	char data[512] = "Test\r\n";
	snprintf(buf, sizeof(buf), "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n%s\r\n",
			(int)strlen(data),
			!http_should_keep_alive(parser) ? "Connection: close\r\n" : "");
	buf_write(&d->fd_state, buf, strlen(buf));
	buf_write(&d->fd_state, data, strlen(data));

	return -1;
}

static int on_url(http_parser *parser, const char *at, size_t length)
{
	UNUSED(parser);
	UNUSED(at); // When in NDEBUG
	UNUSED(length); // When in NDEBUG
	DEBUG("URL: %.*s", (int)length, at);
	return 0;
}

static int on_status(http_parser *parser, const char *at, size_t length)
{
	UNUSED(parser);
	UNUSED(at); // When in NDEBUG
	UNUSED(length); // When in NDEBUG
	DEBUG("STATUS: %.*s", (int)length, at);
	return 0;
}

static int on_header_field(http_parser *parser, const char *at, size_t length)
{
	UNUSED(parser);
	UNUSED(at); // When in NDEBUG
	UNUSED(length); // When in NDEBUG
	DEBUG("HEADER FIELD: %.*s", (int)length, at);
	return 0;
}

static int on_header_value(http_parser *parser, const char *at, size_t length)
{
	UNUSED(parser);
	UNUSED(at); // When in NDEBUG
	UNUSED(length); // When in NDEBUG
	DEBUG("HEADER VALUE: %.*s", (int)length, at);
	return 0;
}

static int on_body(http_parser *parser, const char *at, size_t length)
{
	UNUSED(parser);
	UNUSED(at); // When in NDEBUG
	UNUSED(length); // When in NDEBUG
	DEBUG("BODY: %.*s", (int)length, at);
	return 0;
}

static const struct http_parser_settings parser_settings = {
	.on_message_begin = on_message_begin,
	.on_headers_complete = on_headers_complete,
	.on_message_complete = on_message_complete,

	.on_url = on_url,
	.on_status = on_status,
	.on_header_field = on_header_field,
	.on_header_value = on_header_value,
	.on_body = on_body,
};

static void web_run(void *arg)
{
	struct web_data d = {
		.fd = (long int)arg,
	};
	http_parser parser;

	wire_fd_mode_init(&d.fd_state, d.fd);
	wire_fd_mode_read(&d.fd_state);

	set_nonblock(d.fd);

	http_parser_init(&parser, HTTP_REQUEST);
	parser.data = &d;

	char buf[4096];
	do {
		buf[0] = 0;
		int received = read(d.fd, buf, sizeof(buf));
		DEBUG("Received: %d %d", received, errno);
		if (received == 0) {
			/* Fall-through, tell parser about EOF */
			DEBUG("Received EOF");
		} else if (received < 0) {
			if (errno == EINTR || errno == EAGAIN) {
				DEBUG("Waiting");
				/* Nothing received yet, wait for it */
				wire_fd_wait(&d.fd_state);
				DEBUG("Done waiting");
				continue;
			} else {
				DEBUG("Error receiving from socket %d: %m", d.fd);
				break;
			}
		}

		DEBUG("Processing %d", (int)received);
		size_t processed = http_parser_execute(&parser, &parser_settings, buf, received);
		if (parser.upgrade) {
			/* Upgrade not supported yet */
			xlog("Upgrade no supported, bailing out");
			break;
		} else if (received == 0) {
			// At EOF, exit now
			DEBUG("Received EOF");
			break;
		} else if (processed != (size_t)received) {
			// Error in parsing
			xlog("Not everything was parsed, error is likely, bailing out.");
			break;
		}
	} while (1);

	wire_fd_mode_none(&d.fd_state);
	close(d.fd);
	DEBUG("Disconnected %d", d.fd);
}

static void accept_run(void *arg)
{
	UNUSED(arg);
	int port = 9090;
	int fd = socket_setup(port);
	if (fd < 0)
		return;

	xlog("Listening on port %d", port);

	wire_fd_state_t fd_state;
	wire_fd_mode_init(&fd_state, fd);
	wire_fd_mode_read(&fd_state);

	/* To be as fast as possible we want to accept all pending connections
	 * without waiting in between, the throttling will happen by either there
	 * being no more pending listeners to accept or by the wire pool blocking
	 * when it is exhausted.
	 */
	while (1) {
		int new_fd = accept(fd, NULL, NULL);
		if (new_fd >= 0) {
			DEBUG("New connection: %d", new_fd);
			char name[32];
			snprintf(name, sizeof(name), "web %d", new_fd);
			wire_t *task = wire_pool_alloc_block(&web_pool, name, web_run, (void*)(long int)new_fd);
			if (!task) {
				xlog("Web server is busy, sorry");
				close(new_fd);
			}
		} else {
			if (errno == EINTR || errno == EAGAIN) {
				/* Wait for the next connection */
				wire_fd_wait(&fd_state);
			} else {
				xlog("Error accepting from listening socket: %m");
				break;
			}
		}
	}
}

int main()
{
	wire_thread_init(&wire_thread_main);
	wire_fd_init();
	wire_pool_init(&web_pool, NULL, WEB_POOL_SIZE, 16*1024);
	wire_init(&wire_accept, "accept", accept_run, NULL, WIRE_STACK_ALLOC(4096));
	wire_thread_run();
	return 0;
}
