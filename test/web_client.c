/* Download multiple files as a simple web client. */

#include "wire.h"
#include "wire_fd.h"
#include "wire_net.h"
#include "wire_io.h"
#include "wire_stack.h"
#include "wire_pool.h"

#include "utils.h"

#include <stdio.h>
#include <stdarg.h>
#include <memory.h>

static wire_thread_t wire_main;
static int g_argc;
static char **g_argv;
static wire_t init_wire;
static char wire_init_stack[4096];
static wire_pool_t wire_pool;

static const char req_prefix[] = "GET ";
static const char req_mid[] =
	" HTTP/1.0\r\n"
	"User-Agent: wire-sysmon\r\n"
	"Host: ";
static const char req_suffix[] =
	"\r\n"
	"Accept: */*\r\n"
	"\r\n";

static void wlog(const char *msg, ...)
{
	char buf[128];
	int n;
	va_list ap;

	va_start(ap, msg);
	n = vsnprintf(buf, sizeof(buf), msg, ap);
	va_end(ap);

	// Prevent overflow, cut-off the length and just add newline
	if (n >= (int)sizeof(buf))
		n = sizeof(buf) - 1;
	buf[n] = '\n';

	wio_write(1, buf, n+1);
}

#define HTTP_PROTOCOL "http://"

static int parse_url(char *url, char **phostname, char **pport, char **purl_path)
{
	char *hostname;
	char *url_path;
	char *port;

	if (memcmp(url, HTTP_PROTOCOL, strlen(HTTP_PROTOCOL)) != 0)
		return -1;

	hostname = url + strlen(HTTP_PROTOCOL);
	url_path = strchr(hostname, '/');
	int hostname_len = url_path - hostname;
	memmove(url, hostname, hostname_len);
	hostname = url;
	hostname[hostname_len] = 0;

	port = strchr(hostname, ':');
	if (port) {
		*port = 0;
		port++;
	}

	*phostname = hostname;
	*purl_path = url_path;
	*pport = port ? port : "http";
	return 0;
}

static void dl_wire_func(void *arg)
{
	char *url = arg;
	char *hostname;
	char *url_path;
	char *port;
	int ret;
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int sfd;
	wire_net_t net;
	static int file_idx;
	char filename[32];

	snprintf(filename, sizeof(filename), "save.%d.log", file_idx++);

	if (parse_url(url, &hostname, &port, &url_path) < 0) {
		wlog("Invalid URL %s", url);
		return;
	}

	wlog("Starting download for %s", url);

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
	hints.ai_flags = 0;
	hints.ai_protocol = 0;          /* Any protocol */

	ret = wio_getaddrinfo(hostname, port, &hints, &result);
	if (ret != 0) {
		wlog("Hostname resolution failed for %s port %s", hostname, port);
		return;
	}

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1)
			continue;

		set_nonblock(sfd);

		wire_net_init(&net, sfd);
		wire_timeout_reset(&net.tout, 10000);

		if (wire_net_connect(&net, rp->ai_addr, rp->ai_addrlen) != -1)
			break;                  /* Success */

		wire_net_close(&net);
	}

	freeaddrinfo(result);           /* No longer needed */

	if (rp == NULL) {               /* No address succeeded */
		wlog("Could not connect to %s:%s", hostname, port);
		return;
	}

	wlog("Connected to %s", hostname);

	size_t sent;
	ret = wire_net_write(&net, req_prefix, strlen(req_prefix), &sent);
	if (ret < 0 || sent != strlen(req_prefix)) {
		wlog("Failed to send full request prefix");
		goto Exit;
	}

	ret = wire_net_write(&net, url_path, strlen(url_path), &sent);
	if (ret < 0 || sent != strlen(url_path)) {
		wlog("Failed to send full url path");
		goto Exit;
	}

	ret = wire_net_write(&net, req_mid, strlen(req_mid), &sent);
	if (ret < 0 || sent != strlen(req_mid)) {
		wlog("Failed to send mid request");
		goto Exit;
	}

	ret = wire_net_write(&net, hostname, strlen(hostname), &sent);
	if (ret < 0 || sent != strlen(hostname)) {
		wlog("Failed to send hostname");
		goto Exit;
	}

	ret = wire_net_write(&net, req_suffix, strlen(req_suffix), &sent);
	if (ret < 0 || sent != strlen(req_suffix)) {
		wlog("Failed to send full url path");
		goto Exit;
	}

	wlog("Creating file %s", filename);
	int save_fd = wio_open(filename, O_WRONLY|O_CREAT|O_TRUNC|O_CLOEXEC, 0666);
	char buf[2048];
	size_t rcvd;

	wlog("Reading data for http://%s:%s%s", hostname, port, url_path);

	do {
		ret = wire_net_read_any(&net, buf, sizeof(buf), &rcvd);
		if (ret == 0) {
			sent = wio_write(save_fd, buf, rcvd);
			if (sent != rcvd)
				wlog("Failed to write all data to file %s", filename);
		}
	} while (ret >= 0 && rcvd > 0);

	wlog("Download succeeded for http://%s:%s%s", hostname, port, url_path);

	wio_close(save_fd);

Exit:
	wire_net_close(&net);
	wlog("Finished download for http://%s:%s%s", hostname, port, url_path);
}

static void init_wire_func(void *arg)
{
	UNUSED(arg);

	int i;
	for (i = 1; i < g_argc; i++) {
		wlog("Initiating download for %s", g_argv[i]);
		wire_pool_alloc_block(&wire_pool, g_argv[i], dl_wire_func, g_argv[i]);
	}
	wlog("Done initiating downloads");
}

int main(int argc, char **argv)
{
	wire_thread_init(&wire_main);
	wire_stack_fault_detector_install();
	wire_fd_init();
	wire_io_init(4);

	wire_pool_init(&wire_pool, NULL, 6, 8192);

	g_argc = argc;
	g_argv = argv;
	wire_init(&init_wire, "init", init_wire_func, NULL, wire_init_stack, sizeof(wire_init_stack));

	wire_thread_run();
	return 0;
}
