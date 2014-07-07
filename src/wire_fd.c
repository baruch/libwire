#include "wire.h"
#include "wire_fd.h"
#include "wire_stack.h"

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <time.h>

struct fd_state {
	int epoll_fd;
	int count;
};

static struct fd_state state;

static wire_t fd_wire;

static void wire_fd_action_added(void)
{
	if (state.count == 0) {
		wire_resume(&fd_wire);
	}
	state.count++;
}

static void wire_fd_action_removed(void)
{
	state.count--;
}

static void wire_fd_monitor(void *arg)
{
	UNUSED(arg);
	printf("fd monitor starts\n");

	while (1) {
		if (state.count == 0)
			wire_suspend();

		// Only block if we are the only thing that needs to happen
		int timeout = wire_is_only_one() ? -1 : 0;
		struct epoll_event events[2048/sizeof(struct epoll_event)]; // Don't use more than 2K of stack

		int event_count = epoll_wait(state.epoll_fd, events, ARRAY_SIZE(events), timeout);
		int i;

		for (i = 0; i < event_count; i++) {
			wire_fd_state_t *fd_state = events[i].data.ptr;
			if (!wire_wait_resume(&fd_state->wait)) {
				// If the resume is not needed, disable the fd_state so we won't be in a busy loop
				wire_fd_mode_none(fd_state);
			}
		}

		// Let the just resumed wires and other ready wires get their time
		wire_yield();
	}

	printf("fd monitor ends! (unexpected)\n");
}

void wire_fd_mode_init(wire_fd_state_t *st, int fd)
{
	st->fd = fd;
	st->state = FD_MODE_NONE;
	wire_wait_init(&st->wait);
}

static int wire_fd_mode_switch(wire_fd_state_t *fd_state, wire_fd_mode_e end_mode)
{
	int op;

	if (end_mode == fd_state->state)
		return 0;

	if (end_mode == FD_MODE_NONE)
		op = EPOLL_CTL_DEL;
	else if (fd_state->state == FD_MODE_NONE) {
		op = EPOLL_CTL_ADD;
	} else {
		op = EPOLL_CTL_MOD;
	}

	uint32_t event_code = end_mode == FD_MODE_READ ? EPOLLIN : EPOLLOUT;
	struct epoll_event event = {
		.events = event_code,
		.data.ptr = fd_state
	};
	int ret = epoll_ctl(state.epoll_fd, op, fd_state->fd, &event);
	if (ret >= 0) {
		fd_state->state = end_mode;
		if (op == EPOLL_CTL_ADD)
			wire_fd_action_added();
		else if (op == EPOLL_CTL_DEL)
			wire_fd_action_removed();
	}
	return ret;
}

int wire_fd_mode_read(wire_fd_state_t *fd_state)
{
	return wire_fd_mode_switch(fd_state, FD_MODE_READ);
}

int wire_fd_mode_write(wire_fd_state_t *fd_state)
{
	return wire_fd_mode_switch(fd_state, FD_MODE_WRITE);
}

int wire_fd_mode_none(wire_fd_state_t *fd_state)
{
	return wire_fd_mode_switch(fd_state, FD_MODE_NONE);
}

void wire_fd_wait(wire_fd_state_t *fd_state)
{
	wire_wait_list_t wait_list;

	wire_wait_list_init(&wait_list);
	wire_wait_chain(&wait_list, &fd_state->wait);
	wire_list_wait(&wait_list);
}

int wire_fd_wait_msec(int msecs)
{
	if (msecs <= 0)
		return 0;

	int fd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC|TFD_NONBLOCK);
	if (fd < 0) {
		perror("Failed to create a timerfd");
		return -1;
	}

	struct itimerspec timer = {
		.it_value = { .tv_sec = msecs / 1000, .tv_nsec = (msecs % 1000) * 1000000}
	};

	int ret = timerfd_settime(fd, 0, &timer, NULL);

	wire_wait_list_t wait_list;
	wire_wait_list_init(&wait_list);

	wire_fd_state_t fd_state;
	wire_fd_mode_init(&fd_state, fd);
	wire_fd_mode_read(&fd_state);

	while (1) {
		wire_fd_wait(&fd_state);

		uint64_t timer_val = 0;
		ret = read(fd, &timer_val, sizeof(timer_val));
		if (ret < (int)sizeof(timer_val)) {
			if (errno == EAGAIN)
				continue;
			perror("Error reading from timerfd");
			ret = -1;
			break;
		}

		if (timer_val > 0) {
			ret = 0;
			break;
		}
	}

	wire_fd_mode_none(&fd_state);
	close(fd);
	return ret;
}

void wire_fd_wait_list_chain(wire_wait_list_t *wl, wire_fd_state_t *fd_state)
{
	wire_wait_chain(wl, &fd_state->wait);
}

void wire_fd_init(void)
{
	wire_init(&fd_wire, "fd monitor", wire_fd_monitor, NULL, WIRE_STACK_ALLOC(2*4096));

	state.epoll_fd = epoll_create1(EPOLL_CLOEXEC);
	if (state.epoll_fd < 0) {
		perror("Failed to get an epoll fd: ");
		abort();
	}
}
