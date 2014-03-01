#ifndef XCORO_MACROS_H
#define XCORO_MACROS_H

#include <stddef.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#define container_of(ptr, type, field) ((type*)((void*)ptr - (void*)offsetof(type, field)))

#endif
