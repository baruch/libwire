#include "wire_timeout.h"
#include "wire_io.h"

#include <stdint.h>
#include <sys/timerfd.h>

int wire_timeout_init(wire_timeout_t *tout)
{
	int fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK|TFD_CLOEXEC);
	if (fd < 0)
		return -1;

	wire_fd_mode_init(&tout->fd_state, fd);
	wire_wait_init(&tout->wait);
	return 0;
}

static void clear_timeout(int fd)
{
	// Clear the timeout if it already triggered
	uint64_t val;
	int ret = read(fd, &val, sizeof(val));
	(void)ret;
}

int wire_timeout_reset(wire_timeout_t *tout, int timeout_msec)
{
	struct itimerspec tspec;

	tspec.it_interval.tv_sec = 0;
	tspec.it_interval.tv_nsec = 0;
	tspec.it_value.tv_sec = timeout_msec / 1000;
	timeout_msec %= 1000;
	tspec.it_value.tv_nsec = timeout_msec ? timeout_msec * 1000 * 1000 : 0;

	clear_timeout(tout->fd_state.fd);

	// Set the new timeout
	wire_timeout_wait_start(tout);
	return timerfd_settime(tout->fd_state.fd, 0, &tspec, NULL);
}

void wire_timeout_stop(wire_timeout_t *tout)
{
	wire_fd_mode_none(&tout->fd_state);
	wio_close(tout->fd_state.fd);
}

void wire_timeout_wait_start(wire_timeout_t *tout)
{
	wire_fd_mode_read(&tout->fd_state);
	wire_wait_reset(&tout->fd_state.wait);
}

void wire_timeout_wait_stop(wire_timeout_t *tout)
{
	wire_fd_mode_none(&tout->fd_state);
	clear_timeout(tout->fd_state.fd);
}

int wire_timeout_wait(wire_wait_t *wait_wire, wire_timeout_t *tout)
{
	wire_wait_t *tout_wait = wire_timeout_wait_get(tout);
	return wire_wait_two(wait_wire, tout_wait);
}
