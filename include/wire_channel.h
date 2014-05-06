#ifndef WIRE_LIB_CHANNEL_H
#define WIRE_LIB_CHANNEL_H
/** @file
 */

#include "list.h"
#include "wire.h"
#include "wire_wait.h"

/** @addtogroup Channels
 * A channel is a communication medium. The only channel implementation for now
 * is a block channel of size one, it is used for both message passing and
 * synchronization between coroutines. When sending to a channel the sender
 * blocks until the reader removes the message. If another coroutine wants to
 * send on the channel it will block as well.
 */
/// @{

typedef struct wire_channel wire_channel_t;
typedef struct wire_channel_receiver wire_channel_receiver_t;

/** Initialize a channel before it can be used.
 *
 * @param[in] c The channel to initialize
 */
void wire_channel_init(wire_channel_t *c);

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
void wire_channel_send(wire_channel_t *c, void *msg);

/** Receive a message from the channel, block until a message is received.
 *
 * This will wait for a message to be received and will wait for the sender to
 * wake it up.
 *
 * @param[in] c Channel to receive from
 * @param[in] msg Message received
 * @return 0 if a message is received, -1 on error
 */
int wire_channel_recv_block(wire_channel_t *c, void **msg);

/** Receive a message from the channel, do not block if there is nothing on the channel.
 *
 * @param[in] c Channel to receive from
 * @param[in] msg Message received
 * @return 0 if a message is received, -1 on error
 */
int wire_channel_recv_nonblock(wire_channel_t *c, void **msg);

/** Set the wire to wait on a channel, it will be woken up when there is
 * something to receive on the channel. It is then expected that the receiver
 * will call wire_channel_recv_nonblock().
 *
 * @param[in] c Channel to wait for.
 * @param[in] receiver Channel receiver to use when waiting.
 * @param[in] wait The wait object which is used to receive the notification when the channel has data to receive.
 */
void wire_channel_recv_wait(wire_channel_t *c, wire_channel_receiver_t *receiver, wire_wait_t *wait);

/// @}

struct wire_channel {
	struct list_head pending_send;
	struct list_head pending_recv;
};

struct wire_channel_receiver {
	struct list_head list;
	wire_wait_t *wait;
};

#endif
