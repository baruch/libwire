#include "wire.h"
#include "wire_net.h"
#include "wire_io.h"

#include <errno.h>

static int wait_for_2(wire_wait_t *wait1, wire_wait_t *wait2)
{
	wire_wait_list_t wl;

	wire_wait_list_init(&wl);
	wire_wait_chain(&wl, wait1);
	wire_wait_chain(&wl, wait2);
	wire_list_wait(&wl);

	if (wait1->triggered)
		return 1;
	else if (wait2->triggered)
		return 2;
	else
		return -1;
}

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

int wire_net_connect(wire_net_t *net, const struct sockaddr *addr, socklen_t addrlen)
{
	int ret = connect(net->fd_state.fd, addr, addrlen);
	if (ret == 0) // Immediately connected
		return 0;

	if (errno != EINPROGRESS) // Connection failed
		return -1;

	// Connection is in progress
	wire_wait_t *tout_wait = wire_timeout_wait_get(&net->tout);

	wire_fd_mode_write(&net->fd_state);
	wait_for_2(&net->fd_state.wait, tout_wait);
	wire_fd_mode_none(&net->fd_state);

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
		} else { // Error condition: ret < 0
			if (errno == EAGAIN) {
				// We are waiting for more data and none is present, wait for it but yield the wire
				wire_fd_mode_write(&net->fd_state);
				wire_wait_t *tout_wait = wire_timeout_wait_get(&net->tout);
				ret = wait_for_2(&net->fd_state.wait, tout_wait);
				if (ret != 1) // Not the IO returned
					goto Exit;
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
	wire_fd_mode_none(&net->fd_state);
	*psent = sent;
	return res;
}

int wire_net_read_min(wire_net_t *net, void *buf, size_t count, size_t *prcvd, size_t min_read)
{
	int res = -1;
	size_t rcvd = 0;

	while (rcvd < min_read) {
		int ret = read(net->fd_state.fd, buf + rcvd, count - rcvd);
		if (ret == 0)
			goto Exit;
		else if (ret > 0) {
			rcvd += ret;
		} else { // Error condition: ret < 0
			if (errno == EAGAIN) {
				// We are waiting for more data and none is prercvd, wait for it but yield the wire
				wire_fd_mode_read(&net->fd_state);
				wire_wait_t *tout_wait = wire_timeout_wait_get(&net->tout);
				ret = wait_for_2(&net->fd_state.wait, tout_wait);
				if (ret != 1) // Not the IO returned
					goto Exit;
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
	wire_fd_mode_none(&net->fd_state);
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
