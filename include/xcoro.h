#ifndef XCORO_LIB_H
#define XCORO_LIB_H

#define XCORO_TASK_SIZE sizeof(xcoro_task_t)
typedef struct xcoro xcoro_t;
typedef struct xcoro_task xcoro_task_t;

void xcoro_init(xcoro_t *xcoro);
void xcoro_run(void);
void xcoro_yield(void);
xcoro_task_t *xcoro_get_current_task(void);
int xcoro_is_only_task(void);

xcoro_task_t *xcoro_task_init(xcoro_task_t *task, const char *name, void (*entry_point)(void *), void *task_data, void *stack, unsigned stack_size);

#include "xcoro_private.h"

#endif
