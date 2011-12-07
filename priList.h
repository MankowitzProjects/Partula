#ifndef __LIST_H
#define __LIST_H

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

#define LIST_POSITION_NULL 0

struct list_head {
    struct list_head *next, *prev;
    unsigned long num;
};

#define LIST_HEAD_INIT(name) { &(name), &(name), 0 }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
	list->num  = 0;
}

/*
 * Insert a elem entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_add(struct list_head *elem,
                  struct list_head *prev,
                  struct list_head *next)
{
    next->prev = elem;
    elem->next = next;
    elem->prev = prev;
    prev->next = elem;
}

/**
 * list_add - add a elem entry
 * @elem: elem entry to be added
 * @head: list head to add it after
 *
 * Insert a elem entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void list_add(struct list_head *elem, struct list_head *head)
{
    __list_add(elem, head, head->next);
}

/**
 * list_add_tail - add a elem entry
 * @elem: elem entry to be added
 * @head: list head to add it before
 *
 * Insert a elem entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void list_add_tail(struct list_head *elem, struct list_head *head)
{
    __list_add(elem, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_del(struct list_head *prev, struct list_head *next)
{
    next->prev = prev;
    prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note:   list_empty on entry does not return true after this, the entry is in an undefined state.
 */
static inline void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = LIST_POSITION_NULL;
    entry->prev = LIST_POSITION_NULL;
}

/**
 * list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static inline void list_del_init(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    INIT_LIST_HEAD(entry);
}

/**
 * list_move - delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
static inline void list_move(struct list_head *list, struct list_head *head)
{
    __list_del(list->prev, list->next);
    list_add(list, head);
}

/**
 * list_move_tail - delete from one list and add as another's tail
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
static inline void list_move_tail(struct list_head *list,
                  struct list_head *head)
{
        __list_del(list->prev, list->next);
        list_add_tail(list, head);
}

/**
 * list_is_last - tests whether @list is the last entry in list @head
 * @list: the entry to test
 * @head: the head of the list
 */
static inline int list_is_last(const struct list_head *list,
				const struct list_head *head)
{
	return list->next == head;
}

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static inline int list_empty(const struct list_head *head)
{
    return head->next == head;
}

static inline void __list_splice(const struct list_head *list,
				 struct list_head *prev,
				 struct list_head *next)
{
	struct list_head *first = list->next;
	struct list_head *last = list->prev;

	first->prev = prev;
	prev->next = first;

	last->next = next;
	next->prev = last;
}

/**
 * list_splice - join two lists, this is designed for stacks
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void list_splice(const struct list_head *list,
				struct list_head *head)
{
	if (!list_empty(list))
		__list_splice(list, head, head->next);
}

/**
 * list_splice_tail - join two lists, each list being a queue
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void list_splice_tail(struct list_head *list,
				struct list_head *head)
{
	if (!list_empty(list))
		__list_splice(list, head->prev, head);
}

/**
 * list_splice_init - join two lists and reinitialise the emptied list.
 * @list: the elem list to add.
 * @head: the place to add it in the first list.
 *
 * The list at @list is reinitialised
 */
static inline void list_splice_init(struct list_head *list,
                    struct list_head *head)
{
	if (!list_empty(list)) {
		__list_splice(list, head, head->next);
		INIT_LIST_HEAD(list);
	}
}

/**
 * list_entry - get the struct for this entry
 * @ptr:    the &struct list_head pointer.
 * @type:    the type of the struct this is embedded in.
 * @member:    the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

/**
 * list_for_each    -    iterate over a list
 * @pos:    the &struct list_head to use as a loop counter.
 * @head:   the head for your list.
 */
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); \
            pos = pos->next)
/**
 * list_for_each_prev    -    iterate over a list backwards
 * @pos:    the &struct list_head to use as a loop counter.
 * @head:   the head for your list.
 */
#define list_for_each_prev(pos, head) \
    for (pos = (head)->prev; pos != (head); \
            pos = pos->prev)

/**
 * list_for_each_safe    -    iterate over a list safe against removal of list entry
 * @pos:    the &struct list_head to use as a loop counter.
 * @n:      another &struct list_head to use as temporary storage
 * @head:   the head for your list.
 */
#define list_for_each_safe(pos, n, head)                        \
    for (pos = (head)->next, n = pos->next; pos != (head);      \
        pos = n, n = pos->next)

/**
 * list_for_each_entry    -    iterate over list of given type
 * @pos:    the type * to use as a loop counter.
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, head, member)                      \
    for (pos = list_entry((head)->next, typeof(*pos), member);      \
         &pos->member != (head);                                    \
         pos = list_entry(pos->member.next, typeof(*pos), member))

/**
 * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:    the type * to use as a loop counter.
 * @n:      another type * to use as temporary storage
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define list_for_each_entry_safe(pos, n, head, member)          \
    for (pos = list_entry((head)->next, typeof(*pos), member),  \
        n = list_entry(pos->member.next, typeof(*pos), member); \
         &pos->member != (head);                                \
         pos = n, n = list_entry(n->member.next, typeof(*n), member))



/** \brief Insert an element by comparing the priority number.
 *         The one with lager number will be placed in front of the list.
 *
 * \param elem struct list_head*
 * \param head struct list_head*
 * \return void
 *
 */
static inline void list_pri_add(struct list_head *elem, struct list_head *head)
{
    list_head *pos = NULL;

    list_for_each(pos, head)
    {
        // comparing the priority
        if (elem->num >= pos->num)
        {
            break;
        }
    }

    // insert the element
    list_add(elem, pos);
}

/** \brief Check if an element is in the list.
 *
 * \param elem list_head*
 * \param head list_head*
 * \return bool
 *
 */
static inline bool listElemInList(list_head *elem, list_head *head)
{
    list_head *pos = NULL;

    list_for_each(pos, head)
    {
        if (pos == elem)
        {
            return true;
        }
    }

    return false;
}

#endif
