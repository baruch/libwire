#ifndef WIRE_SEM_LIB_H
#define WIRE_SEM_LIB_H

#include "list.h"

typedef struct wire_sem {
	int count;
	struct list_head waiters;
} wire_sem_t;

void wire_sem_init(wire_sem_t *sem, int count);
void wire_sem_take(wire_sem_t *sem);
void wire_sem_release(wire_sem_t *sem);

#endif
