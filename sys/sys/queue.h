#ifndef	SYS_QUEUE_H
#define	SYS_QUEUE_H

/*
 * BSD-like singly-linked lists and lists.
 *
 * +---------------+-------+------+
 * |               | SLIST | LIST |
 * +---------------+-------+------+
 * | HEAD          |   1   |  1   |
 * | ENTRY         |   1   |  1   |
 * |               |       |      |
 * | FIRST         |   1   |  1   |
 * | NEXT          |   1   |  1   |
 * | PREV          |   0   |  1   |
 * | FOREACH       |   1   |  1   |
 * |               |       |      |
 * | INIT          |   1   |  1   |
 * | EMPTY         |   1   |  1   |
 * |               |       |      |
 * | INSERT_HEAD   |   1   |  1   |
 * | INSERT_AFTER  |   1   |  1   |
 * | INSERT_BEFORE |   0   |  1   |
 * |               |       |      |
 * | REMOVE_HEAD   |   1   |  0   |
 * | REMOVE_AFTER  |   1   |  0   |
 * | REMOVE        |   0   |  1   |
 * +------------------------------+
 */

/*
 * Singly-linked list defs:
 */

#define	SLIST_HEAD(name, type)                                                \
	struct name {                                                         \
		struct type *first;                                           \
	}                                                                     \

#define	SLIST_ENTRY(type)                                                     \
	struct {                                                              \
		struct type *next;                                            \
	}

/*
 * Singly-linked list access:
 */

#define	SLIST_FIRST(head)                                                     \
	((head)->first)

#define	SLIST_NEXT(entry, field)                                              \
	((entry)->field.next)

#define	SLIST_FOREACH(iter, head, field)                                      \
	for ((iter) = SLIST_FIRST(head);                                      \
	     (iter);                                                          \
	     (iter) = SLIST_NEXT(iter, field))

/*
 * Singly-linked list ops:
 */

#define	SLIST_INIT(head)                                                      \
	(SLIST_FIRST(head) = 0)

#define	SLIST_EMPTY(head)                                                     \
	(SLIST_FIRST(head) == 0)

#define	SLIST_INSERT_HEAD(head, entry, field) do {                            \
	SLIST_NEXT(entry, field) = SLIST_FIRST(head);                         \
	SLIST_FIRST(head) = (entry);                                          \
} while (0)

#define	SLIST_INSERT_AFTER(ent0, ent1, field) do {                            \
	SLIST_NEXT(ent1, field) = SLIST_NEXT(ent0, field);                    \
	SLIST_NEXT(ent0, field) = (ent1);                                     \
} while (0)

#define	SLIST_REMOVE_HEAD(head, field)                                        \
	(SLIST_FIRST(head) = SLIST_NEXT(SLIST_FIRST(head), field))

#define	SLIST_REMOVE_AFTER(entry, field)                                      \
	(SLIST_NEXT(entry, field) = SLIST_NEXT(SLIST_NEXT(entry, field),field))

/*
 * Doubly-linked list defs:
 */

#define	LIST_HEAD(name, type)                                                 \
	struct name {                                                         \
		struct type *first;                                           \
	}

#define	LIST_ENTRY(type)                                                      \
	struct {                                                              \
		struct type  *next;                                           \
		struct type **prev;                                           \
	}

/*
 * Doubly-linked list access:
 */

#define	LIST_FIRST(head)                                                      \
	((head)->first)

#define	LIST_NEXT(entry, field)                                               \
	((entry)->field.next)

#define	LIST_PREV(entry, field)                                               \
	((entry)->field.prev)

#define	LIST_FOREACH(iter, head, field)                                       \
	for ((iter) = LIST_FIRST(head);                                       \
	     (iter);                                                          \
	     (iter) = LIST_NEXT(iter, field))

/*
 * Doubly-linked list ops:
 */

#define	LIST_INIT(head)                                                       \
	(LIST_FIRST(head) = 0)

#define	LIST_EMPTY(head)                                                      \
	(LIST_FIRST(head) == 0)

#define	LIST_INSERT_HEAD(head, entry, field) do {                             \
	LIST_NEXT(entry, field) = LIST_FIRST(head);                           \
	if (!LIST_EMPTY(head))                                                \
		LIST_PREV(LIST_FIRST(head), field) = &LIST_NEXT(entry, field);\
	LIST_FIRST(head) = (entry);                                           \
	LIST_PREV(entry, field) = &LIST_FIRST(head);                          \
} while (0)

#define	LIST_INSERT_BEFORE(ent0, ent1, field) do {                            \
	LIST_PREV(ent1, field) = LIST_PREV(ent0, field);                      \
	LIST_NEXT(ent1, field) = (ent0);                                      \
	*LIST_PREV(ent0, field) = (ent1);                                     \
	LIST_PREV(ent0, field) = &LIST_NEXT(ent1, field);                     \
} while (0)

#define	LIST_INSERT_AFTER(ent0, ent1, field) do {                             \
	LIST_NEXT(ent1, field) = LIST_NEXT(ent0, field);                      \
	if (LIST_NEXT(ent0, field))                                           \
		LIST_PREV(LIST_NEXT(ent0, field), field) =                    \
		    &LIST_NEXT(ent1, field);                                  \
	LIST_NEXT(ent0, field) = (ent1);                                      \
	LIST_PREV(ent1, field) = &LIST_NEXT(ent0, field);                     \
} while (0)

#define	LIST_REMOVE(entry, field) do {                                        \
	if (LIST_NEXT(entry, field))                                          \
		LIST_PREV(LIST_NEXT(entry, field), field) =                   \
		    LIST_PREV(entry, field);                                  \
	if (LIST_PREV(entry, field))                                          \
		*LIST_PREV(entry, field) = LIST_NEXT(entry, field);           \
} while (0)

#endif
