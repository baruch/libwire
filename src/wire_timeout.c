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

int wire_timeout_reset(wire_timeout_t *tout, int timeout_msec)
{
	struct itimerspec tspec;

	tspec.it_interval.tv_sec = 0;
	tspec.it_interval.tv_nsec = 0;
	tspec.it_value.tv_sec = timeout_msec / 1000;
	timeout_msec %= 1000;
	if (timeout_msec)
		tspec.it_value.tv_nsec = timeout_msec * 1000 * 1000;

	// Clear the timeout if it already triggered
	uint64_t val;
	read(tout->fd_state.fd, &val, sizeof(val));

	// Set the new timeout
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
}
