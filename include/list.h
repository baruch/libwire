#ifndef WIRE_LIST_H
#define WIRE_LIST_H

#include "macros.h"

struct list_head {
	struct list_head *next;
	struct list_head *prev;
};

static inline void list_head_init(struct list_head *head)
{
	head->prev = head->next = head;
}

static inline void _list_add(struct list_head *item, struct list_head *prev, struct list_head *next)
{
	item->next = next;
	item->prev = prev;
	next->prev = item;
	prev->next = item;
}

static inline void list_add_head(struct list_head *item, struct list_head *head)
{
	_list_add(item, head, head->next);
}

static inline void list_add_tail(struct list_head *item, struct list_head *head)
{
	_list_add(item, head->prev, head);
}

static inline void _list_del(struct list_head *prev, struct list_head *next)
{
	prev->next = next;
	next->prev = prev;
}

static inline void list_del(struct list_head *item)
{
	_list_del(item->prev, item->next);
	item->prev = item->next = (void*)0xDEADBEAF;
}

static inline void list_move_head(struct list_head *item, struct list_head *head)
{
	_list_del(item->prev, item->next);
	list_add_head(item, head);
}

static inline void list_move_tail(struct list_head *item, struct list_head *head)
{
	_list_del(item->prev, item->next);
	list_add_tail(item, head);
}

static inline int list_empty(struct list_head *head)
{
	return head->prev == head;
}

static inline int list_is_single(struct list_head *head)
{
	return !list_empty(head) && head->next == head->prev;
}

static inline struct list_head *list_head(struct list_head *head)
{
	if (list_empty(head))
		return NULL;
	return head->next;
}

#define list_entry(ptr, type, field) container_of(ptr, type, field)

#endif
