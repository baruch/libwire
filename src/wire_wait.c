#include "wire_wait.h"
#include "wire.h"

void wire_wait_list_init(wire_wait_list_t *wl)
{
	list_head_init(&wl->head);
}

void wire_wait_init(wire_wait_t *w)
{
	w->list.next = NULL;
	w->list.prev = NULL;
	w->wire = wire_get_current();
	w->waiting = 0;
	w->triggered = 0;
}

void wire_wait_chain(wire_wait_list_t *wl, wire_wait_t *w)
{
	list_add_tail(&w->list, &wl->head);
	w->wire = wire_get_current();
	w->waiting = 1;
	w->triggered = 0;
}

void wire_wait_unchain(wire_wait_t *w)
{
	list_del(&w->list);
}

wire_wait_t *wire_list_wait(wire_wait_list_t *wl)
{
	struct list_head *cur;

	while (1) {
		for (cur = wl->head.next; cur != &wl->head; cur = cur->next) {
			wire_wait_t *cur_w = list_entry(cur, wire_wait_t, list);
			if (cur_w->triggered)
				return cur_w;
		}

		wire_suspend();
	}
}

int wire_wait_resume(wire_wait_t *w)
{
	if (w->waiting && !w->triggered) {
		w->triggered = 1;
		wire_resume(w->wire);
		return 1;
	}

	return 0;
}

void wire_wait_reset(wire_wait_t *w)
{
	w->waiting = 1;
	w->triggered = 0;
}

void wire_wait_stop(wire_wait_t *w)
{
	w->waiting = 0;
	w->triggered = 0;
}

void wire_wait_single(wire_wait_t *w)
{
	wire_wait_list_t wl;

	wire_wait_list_init(&wl);
	wire_wait_chain(&wl, w);
	wire_list_wait(&wl);

	wire_wait_stop(w);
}

int wire_wait_two(wire_wait_t *wait1, wire_wait_t *wait2)
{
	wire_wait_list_t wl;

	wire_wait_list_init(&wl);
	wire_wait_chain(&wl, wait1);
	wire_wait_chain(&wl, wait2);
	wire_list_wait(&wl);

	int ret = -1;
	if (wait1->triggered)
		ret = 1;
	else if (wait2->triggered)
		ret = 2;

	wire_wait_stop(wait1);
	wire_wait_stop(wait2);
	return ret;
}
