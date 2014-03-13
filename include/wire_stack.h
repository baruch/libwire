#ifndef WIRE_STACK_LIB_H
#define WIRE_STACK_LIB_H

/** @file
 * libwire stack allocator.
 */

/** @defgroup Stack Stack Allocator
 * The stack allocator provides a safe stack in the sense that it is surrounded
 * by non-writable/non-readable memory pages and so if there is a stack
 * overflow or underflow the program will crash rather than silently corrupt
 * data.
 * As such all stacks are allocated in multiples of a page size (normally 4K on X86-64 or X86-32).
 *
 * Waste is minimized as much as possible by not touching the pages until they
 * are needed and so the physical memory is not allocated until it is actually
 * written to and the protection pages only use address space but no physical
 * memory.
 */
/// @{

/** Allocate a stack from the stack pool. The stack_size is rounded up to be a
 * multiple of the architecture page size and a protection zone of one page is
 * placed before and after each stack.
 */
void *wire_stack_alloc(unsigned stack_size);

/** A convenience method to provide as an argument to wire_init(). This is to be used as:
 * wire_init(&task, "name", entry_point, arg, WIRE_STACK_ALLOC(4096));
 */
#define WIRE_STACK_ALLOC(size) wire_stack_alloc(size), size


/** Setup a stack overflow/underflow monitor detector. The stack
 * over/under-flow detector will setup a SIGSEGV handler that attempts to
 * detect a segmentation fault caused by an access within a boundary of the
 * existing stack and alert to that to make understanding the failure easier on
 * the developer.
 */
void wire_stack_fault_detector_install(void);

/// @}

#endif
