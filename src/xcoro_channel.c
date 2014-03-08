#include "xcoro_channel.h"
#include "xcoro.h"

struct xcoro_msg {
	struct list_head list;
	xcoro_task_t *caller;
	void *msg;
};

void xcoro_channel_init(xcoro_channel_t *c)
{
	list_head_init(&c->pending);
	c->task = NULL;
}

void xcoro_channel_send(xcoro_channel_t *c, void *msg)
{
	struct xcoro_msg xmsg;

	xmsg.caller = xcoro_get_current_task();
	xmsg.msg = msg;
	list_add_tail(&xmsg.list, &c->pending);

	if (c->task)
		xcoro_resume(c->task);

	do {
		xcoro_suspend();
	} while (!list_empty(&xmsg.list));
}

int xcoro_channel_recv(xcoro_channel_t *c, void **msg)
{
	while (xcoro_channel_recv_nonblock(c, msg) != 0)
	{
		c->task = xcoro_get_current_task();
		xcoro_suspend();
		c->task = NULL;
	}

	return 0;
}

int xcoro_channel_recv_nonblock(xcoro_channel_t *c, void **msg)
{
	struct list_head *list = list_head(&c->pending);
	if (list == NULL)
		return -1;

	list_del(list);

	struct xcoro_msg *xmsg = list_entry(list, struct xcoro_msg, list);
	list_head_init(&xmsg->list); // Tell the caller we have taken the data
	xcoro_resume(xmsg->caller);

	*msg = xmsg->msg;
	return 0;
}
