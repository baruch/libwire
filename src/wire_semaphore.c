#include "wire_semaphore.h"
#include "wire.h"
#include "wire_wait.h"

struct waiter {
	wire_wait_t wait;
	struct list_head list;
};

void wire_sem_init(wire_sem_t *sem, int count)
{
	sem->count = count;
	list_head_init(&sem->waiters);
}

void wire_sem_take(wire_sem_t *sem)
{
	if (sem->count == 0) {
		/* To maintain strict ordering between users we do not increase the
		 * count and let some random wire take the semaphore, we avoid
		 * increasing the counter and immediately wakeup the next waiter.
		 *
		 * When that waiter (us) get woken up there is no need to play with the counter
		 */
		struct waiter waiter;
		wire_wait_init(&waiter.wait);
		list_add_tail(&waiter.list, &sem->waiters);
		wire_wait_single(&waiter.wait);
	} else {
		sem->count--;
	}
}

void wire_sem_release(wire_sem_t *sem)
{
	if (list_empty(&sem->waiters)) {
		sem->count++;
	} else {
		// Someone is waiting for the lock, wake the first user
		struct list_head *next = list_head(&sem->waiters);

		// Remove it from the queue, it now has the semaphore
		list_del(next);

		struct waiter *w = list_entry(next, struct waiter, list);
		wire_wait_resume(&w->wait);
	}
}
