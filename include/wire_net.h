#ifndef WIRE_NET_LIB_H
#define WIRE_NET_LIB_H

#include "wire_fd.h"
#include "wire_timeout.h"

#include <sys/types.h>
#include <sys/socket.h>

/** @file
 * libwire simplified networking calls.
 */

/** @defgroup Net Simplified Networking
 *
 * wire_net.h provides simplified networking services to connect, read, write
 * and handle timeouts on the networking services.
 */
/// @{

/** State of a network connection, includes the file descriptor and timeout. */
typedef struct wire_net {
	wire_fd_state_t fd_state;
	wire_timeout_t tout;
} wire_net_t;

/** Initialize a network socket.
 * @param[in] net The network socket to initialize.
 * @param[in] sockfd The socket on which to operate. It is assumed that the sockfd is already set to be a non-blocking socket.
 */
void wire_net_init(wire_net_t *net, int sockfd);

/** Close a network socket.
 * @param[in] net The network socket to close.
 */
void wire_net_close(wire_net_t *net);

/** Connect to a remote service with a timeout.
 * @param[in] sockfd The socket on which to connect. It is assumed that the sockfd is already set to be a non-blocking socket.
 * @param[in] addr Address to connect to.
 * @param[in] addrlen Address struct size.
 * @param[in] tout Timeout object.
 * @return 0 on success, -1 on error.
 */
int wire_net_connect(wire_net_t *net, const struct sockaddr *addr, socklen_t addrlen);


int wire_net_read_full(wire_net_t *net, void *buf, size_t count, size_t *prcvd);
int wire_net_read_any(wire_net_t *net, void *buf, size_t count, size_t *prcvd);
int wire_net_read_min(wire_net_t *net, void *buf, size_t count, size_t *prcvd, size_t min_read);
int wire_net_write(wire_net_t *net, const void *buf, size_t count, size_t *psent);

/// @}

#endif
