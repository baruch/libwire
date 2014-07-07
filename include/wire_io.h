/** @file
 */
#ifndef WIRE_LIB_IO_H
#define WIRE_LIB_IO_H

/** @addtogroup IO
 * Async IO emulation for those IO operations that do not support async io by
 * sending them to a thread and suspending the wire until the operation is
 * done. This however will not suspend the entire thread running the other
 * wires and enable concurrent operations.
 */
/// @{

/** Initialize the wire async IO support.
 * @param num_threads the number of threads to open to handle the async io requests.
 */
void wire_io_init(int num_threads);

#include "wire_io_gen.h"

/// @}

#endif
