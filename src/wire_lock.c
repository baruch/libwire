#include "wire_lock.h"
#include "wire_wait.h"

struct lock_user {
	wire_wait_t wait;
	struct list_head list;
};

void wire_lock_init(wire_lock_t *l)
{
	l->num_users = 0;
	list_head_init(&l->users);
}

void wire_lock_take(wire_lock_t *l)
{
	l->num_users++;

	if (l->num_users > 1) {
		// We are not the first on the lock, wait for it
		struct lock_user u;

		wire_wait_init(&u.wait);
		list_add_tail(&u.list, &l->users);
		wire_wait_single(&u.wait);

		// When we reach here we were woken up by the wire_wait
	}
}

void wire_lock_release(wire_lock_t *l)
{
	l->num_users--;

	if (l->num_users) {
		// Someone is waiting for the lock, wake the first user
		struct list_head *next = list_head(&l->users);
		// Remove it from the queue, it now has the lock
		list_del(next);

		struct lock_user *u = list_entry(next, struct lock_user, list);
		wire_wait_resume(&u->wait);
	}
}

void wire_lock_wait_clear(wire_lock_t *l)
{
	while (l->num_users) {
		// We wait at the end of the current list and hope that no one joins
		wire_lock_take(l);
		wire_lock_release(l);
		// If someone does try to take the look after us, we'll loop again
	}
}
