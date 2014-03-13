#include "wire_channel.h"
#include "wire.h"

struct wire_msg {
	struct list_head list;
	wire_t *caller;
	void *msg;
};

void wire_channel_init(wire_channel_t *c)
{
	list_head_init(&c->pending);
	c->wire = NULL;
}

void wire_channel_send(wire_channel_t *c, void *msg)
{
	struct wire_msg xmsg;

	xmsg.caller = wire_get_current();
	xmsg.msg = msg;
	list_add_tail(&xmsg.list, &c->pending);

	if (c->wire)
		wire_resume(c->wire);

	do {
		wire_suspend();
	} while (!list_empty(&xmsg.list));
}

int wire_channel_recv(wire_channel_t *c, void **msg)
{
	while (wire_channel_recv_nonblock(c, msg) != 0)
	{
		c->wire = wire_get_current();
		wire_suspend();
		c->wire = NULL;
	}

	return 0;
}

int wire_channel_recv_nonblock(wire_channel_t *c, void **msg)
{
	struct list_head *list = list_head(&c->pending);
	if (list == NULL)
		return -1;

	list_del(list);

	struct wire_msg *xmsg = list_entry(list, struct wire_msg, list);
	list_head_init(&xmsg->list); // Tell the caller we have taken the data
	wire_resume(xmsg->caller);

	*msg = xmsg->msg;
	return 0;
}
