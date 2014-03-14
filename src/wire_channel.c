#include "wire_channel.h"
#include "wire.h"

struct wire_msg {
	struct list_head list;
	wire_t *caller;
	void *msg;
};

void wire_channel_init(wire_channel_t *c)
{
	list_head_init(&c->pending_send);
	list_head_init(&c->pending_recv);
}

void wire_channel_send(wire_channel_t *c, void *msg)
{
	struct wire_msg xmsg;

	// Prepare the message
	list_add_tail(&xmsg.list, &c->pending_send);
	xmsg.caller = wire_get_current();
	xmsg.msg = msg;

	// Wake up the first receiver if it is pending
	if (!list_empty(&c->pending_recv)) {
		struct list_head *ptr = list_head(&c->pending_recv);
		wire_channel_receiver_t *rcvr = list_entry(ptr, wire_channel_receiver_t, list);
		list_del(ptr);

		wire_wait_resume(rcvr->wait);
	}

	// Wait for the list to be empty as a sign it was actually received
	do {
		wire_suspend();
	} while (!list_empty(&xmsg.list));
}

int wire_channel_recv_block(wire_channel_t *c, void **msg)
{
	while (wire_channel_recv_nonblock(c, msg) != 0)
	{
		wire_channel_receiver_t rcvr;
		wire_wait_list_t wait_list;
		wire_wait_t wait;
		wire_wait_list_init(&wait_list);
		wire_wait_init(&wait);
		wire_wait_chain(&wait_list, &wait);

		wire_channel_recv_wait(c, &rcvr, &wait);

		wire_list_wait(&wait_list);
	}

	return 0;
}

void wire_channel_recv_wait(wire_channel_t *c, wire_channel_receiver_t *receiver, wire_wait_t *wait)
{
	receiver->wait = wait;
	list_add_head(&c->pending_recv, &receiver->list);
}

int wire_channel_recv_nonblock(wire_channel_t *c, void **msg)
{
	struct list_head *list = list_head(&c->pending_send);
	if (list == NULL)
		return -1;

	list_del(list);

	struct wire_msg *xmsg = list_entry(list, struct wire_msg, list);
	list_head_init(&xmsg->list); // Tell the caller we have taken the data

	// It is assumed that the message will be consumed before the next time the sender gets cpu time
	wire_resume(xmsg->caller);

	*msg = xmsg->msg;
	return 0;
}
