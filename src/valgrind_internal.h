#ifndef WIRE_VALGRIND_INTERNAL_H
#define WIRE_VALGRIND_INTERNAL_H

#ifdef USE_VALGRIND

#include <valgrind.h>
#include <memcheck.h>

#else

#define VALGRIND_STACK_REGISTER(start, end) 0
#define VALGRIND_MAKE_MEM_UNDEFINED(_ptr, _size)

#endif

#endif
