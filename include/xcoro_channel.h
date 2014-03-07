#ifndef XCORO_LIB_CHANNEL_H
#define XCORO_LIB_CHANNEL_H
/** @file
 */

#include "list.h"
#include "xcoro.h"

/** @addtogroup Channels
 * A channel is a communication medium. The only channel implementation for now
 * is a block channel of size one, it is used for both message passing and
 * synchronization between coroutines. When sending to a channel the sender
 * blocks until the reader removes the message. If another coroutine wants to
 * send on the channel it will block as well.
 */
/// @{

typedef struct xcoro_channel xcoro_channel_t;

/** Initialize a channel before it can be used.
 *
 * @param[in] c The channel to initialize
 */
void xcoro_channel_init(xcoro_channel_t *c);

/** Send a message on the channel. The pointer itself is shuttled across the
 * channel and is expected to be used by the receiver, hence it must not be
 * modified and must persist until the other side has finished its action on
 * it.
 *
 * The actual semantics depend on the sender and the receiver, in the normal
 * case the pointer will be on the stack of the sender and the receiver is
 * expected to not use it after the first yield. It is also possible for the
 * semantics to be that the sender gives a pointer on the heap to the receiver
 * and the receiver will free the memory.
 *
 * @param[in] c Channel to send on
 * @param[in] msg Message to send
 */
void xcoro_channel_send(xcoro_channel_t *c, void *msg);

/** Receive a message from the channel, block until a message is received.
 *
 * This will wait for a message to be received and will wait for the sender to
 * wake it up.
 *
 * @param[in] c Channel to receive from
 * @param[in] msg Message received
 * @return 0 if a message is received, -1 on error
 */
int xcoro_channel_recv(xcoro_channel_t *c, void **msg);

/** Receive a message from the channel, do not block if there is nothing on the channel.
 *
 * @param[in] c Channel to receive from
 * @param[in] msg Message received
 * @return 0 if a message is received, -1 on error
 */
int xcoro_channel_recv_nonblock(xcoro_channel_t *c, void **msg);

/// @}

struct xcoro_channel {
	struct list_head pending;
	xcoro_task_t *task;
};

#endif
