#include "wire.h"
#include "wire_fd.h"
#include "wire_stack.h"

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

static wire_task_t fd_task;

static void wire_fd_action_added(void)
{
	if (state.count == 0) {
		wire_resume(&fd_task);
	}
	state.count++;
}

static void wire_fd_action_removed(void)
{
	state.count--;
}

static int wire_fd_one_shot_action(int fd, uint32_t event_type)
{
	int ret;
	struct epoll_event event;

	event.events = event_type | EPOLLONESHOT;
	event.data.ptr = wire_get_current_task();

	ret = epoll_ctl(state.epoll_fd, EPOLL_CTL_ADD, fd, &event);
	if (ret < 0)
		return -1;

	wire_fd_action_added();
	wire_suspend();
	wire_fd_action_removed();

	ret = epoll_ctl(state.epoll_fd, EPOLL_CTL_DEL, fd, &event);
	return 0;
}

static void wire_fd_monitor(void *arg)
{
	UNUSED(arg);
	printf("fd monitor starts\n");

	while (1) {
		if (state.count == 0)
			wire_suspend();

		// Only block if we are the only thing that needs to happen
		int timeout = wire_is_only_task() ? -1 : 0;
		struct epoll_event events[2048/sizeof(struct epoll_event)]; // Don't use more than 2K of stack

		int event_count = epoll_wait(state.epoll_fd, events, ARRAY_SIZE(events), timeout);
		int i;

		for (i = 0; i < event_count; i++) {
			wire_task_t *task = events[i].data.ptr;
			wire_resume(task);
		}

		// Let the just resumed tasks and other ready tasks get their time
		wire_yield();
	}

	printf("fd monitor ends! (unexpected)\n");
}

int wire_fd_wait_read(int fd)
{
	return wire_fd_one_shot_action(fd, EPOLLIN);
}

void wire_fd_mode_init(wire_fd_state_t *st, int fd)
{
	st->fd = fd;
	st->state = FD_MODE_NONE;
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
		.data.ptr = wire_get_current_task()
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
	if (fd_state->state != FD_MODE_NONE)
		wire_suspend();
}

int wire_fd_wait_msec(int msecs)
{
	int fd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC|TFD_NONBLOCK);
	if (fd < 0) {
		perror("Failed to create a timerfd");
		return -1;
	}

	struct itimerspec timer = {
		.it_value = { .tv_sec = msecs / 1000, .tv_nsec = (msecs % 1000) * 1000000}
	};

	int ret = timerfd_settime(fd, 0, &timer, NULL);

	while (ret >= 0 && wire_fd_wait_read(fd) >= 0) {
		uint64_t timer_val = 0;
		ret = read(fd, &timer_val, sizeof(timer_val));
		if (ret < (int)sizeof(timer_val)) {
			perror("Error reading from timerfd");
			ret = -1;
			break;
		}

		if (timer_val > 0) {
			ret = 0;
			break;
		}
	}

	close(fd);
	return ret;
}

void wire_fd_init(void)
{
	wire_task_init(&fd_task, "fd monitor", wire_fd_monitor, NULL, XCORO_STACK_ALLOC(2*4096));

	state.epoll_fd = epoll_create1(EPOLL_CLOEXEC);
	if (state.epoll_fd < 0) {
		perror("Failed to get an epoll fd: ");
		abort();
	}
}
