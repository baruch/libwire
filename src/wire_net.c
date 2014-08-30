#include "wire.h"
#include "wire_net.h"
#include "wire_io.h"

#include <errno.h>
#include <memory.h>

void wire_net_init(wire_net_t *net, int sockfd)
{
	wire_fd_mode_init(&net->fd_state, sockfd);
	wire_timeout_init(&net->tout);
}

void wire_net_close(wire_net_t *net)
{
	wire_timeout_stop(&net->tout);
	wire_fd_mode_none(&net->fd_state);
	wio_close(net->fd_state.fd);
}

int wire_net_init_tcp_connected(wire_net_t *net, const char *hostname, const char *servicename, int timeout_msec, struct sockaddr *sockaddr, socklen_t *sockaddr_len)
{
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int ret;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
	hints.ai_flags = 0;
	hints.ai_protocol = 0;          /* Any protocol */
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;

	ret = wio_getaddrinfo(hostname, servicename, &hints, &result);
	if (ret != 0)
		return -1;

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		int sfd = socket(rp->ai_family, rp->ai_socktype|SOCK_NONBLOCK|SOCK_CLOEXEC, rp->ai_protocol);
		if (sfd == -1)
			continue;

		wire_net_init(net, sfd);
		wire_timeout_reset(&net->tout, timeout_msec);

		ret = wire_net_connect(net, rp->ai_addr, rp->ai_addrlen);
		if (ret == 0) {
			if (sockaddr && sockaddr_len) {
				if (*sockaddr_len >= rp->ai_addrlen) {
					memcpy(sockaddr, rp->ai_addr, rp->ai_addrlen);
					*sockaddr_len = rp->ai_addrlen;
				} else {
					*sockaddr_len = 0;
				}
			}
			break;                  /* Success */
		}

		wire_net_close(net);
	}

	freeaddrinfo(result);           /* No longer needed */

	if (rp == NULL) // No address succeeded
		return -1;

	return 0;
}

int wire_net_connect(wire_net_t *net, const struct sockaddr *addr, socklen_t addrlen)
{
	int ret = connect(net->fd_state.fd, addr, addrlen);
	if (ret == 0) // Immediately connected
		return 0;

	if (errno != EINPROGRESS) // Connection failed
		return -1;

	// Connection is in progress
	wire_fd_mode_write(&net->fd_state);
	wire_timeout_wait(&net->fd_state.wait, &net->tout);

	// We have reached here after a connect or a timeout
	int error;
	socklen_t error_len = sizeof(error);
	ret = getsockopt(net->fd_state.fd, SOL_SOCKET, SO_ERROR, &error, &error_len);
	if (ret < 0)
		return -1;

	if (error != 0) {
		errno = error;
		return -1;
	}

	return 0;
}

int wire_net_write(wire_net_t *net, const void *buf, size_t count, size_t *psent)
{
	int res = -1;
	size_t sent = 0;

	while (sent < count) {
		int ret = write(net->fd_state.fd, buf + sent, count - sent);
		if (ret == 0)
			goto Exit;
		else if (ret > 0) {
			sent += ret;
			// For fairness we should yield here to let others do their work
			if (sent != count)
				wire_yield();
		} else { // Error condition: ret < 0
			if (errno == EAGAIN) {
				// We are waiting for more data and none is present, wait for it but yield the wire
				wire_fd_mode_write(&net->fd_state);
				ret = wire_timeout_wait(&net->fd_state.wait, &net->tout);
				if (ret != 1) { // Not the IO returned
					wire_timeout_wait_stop(&net->tout);
					goto Exit;
				}
			} else if (errno == EINTR) {
				continue;
			} else {
				// Real error in writing, socket is dead.
				goto Exit;
			}
		}
	}

	res = 0;

Exit:
	*psent = sent;
	return res;
}

int wire_net_read_min(wire_net_t *net, void *buf, size_t count, size_t *prcvd, size_t min_read)
{
	int res = -1;
	size_t rcvd = 0;

	while (rcvd < min_read) {
		int ret = read(net->fd_state.fd, buf + rcvd, count - rcvd);
		if (ret == 0) {
			errno = ENODATA;
			goto Exit;
		} else if (ret > 0) {
			rcvd += ret;
			// For fairness we should yield here to let others do their work
			if (rcvd < min_read)
				wire_yield();
		} else { // Error condition: ret < 0
			if (errno == EAGAIN) {
				// We are waiting for more data and none is prercvd, wait for it but yield the wire
				wire_fd_mode_read(&net->fd_state);
				ret = wire_timeout_wait(&net->fd_state.wait, &net->tout);
				if (ret != 1) {// Not the IO returned
					wire_timeout_wait_stop(&net->tout);
					errno = ETIMEDOUT;
					goto Exit;
				}
			} else if (errno == EINTR) {
				continue;
			} else {
				// Real error in writing, socket is dead.
				goto Exit;
			}
		}
	}

	res = 0;

Exit:
	*prcvd = rcvd;
	return res;
}

int wire_net_read_full(wire_net_t *net, void *buf, size_t count, size_t *prcvd)
{
	return wire_net_read_min(net, buf, count, prcvd, count);
}

int wire_net_read_any(wire_net_t *net, void *buf, size_t count, size_t *prcvd)
{
	return wire_net_read_min(net, buf, count, prcvd, 1);
}
