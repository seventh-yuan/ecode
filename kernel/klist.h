#ifndef __KLIST_H__
#define __KLIST_H__
#include "../global/include.h"

#ifdef __cplusplus
extern "C"{
#endif

#define KLIST_HEAD_INIT(name)    {&(name), &(name)}
#define KLIST_HEAD(name)\
        struct klist_head name = KLIST_HEAD_INIT(name)

#define LIST_INIT(name)\
        struct klist_head name = KLIST_HEAD_INIT(name)

#define klist_for_each(pos, head)\
        for(pos=(head)->next;pos!=(head);pos=pos->next)
            
#define klist_for_each_safe(pos, n, head)\
        for(pos = (head)->next,n=pos->next;pos!=(head);\
            pos=n,n=pos->next)

/**
 * @brief get the struct for this entry
 * @param node the entry point
 * @param type the type of structure
 * @param member the name of klist in structure
 */
#define klist_entry(node, type, member) \
    ((type *)((char *)(node) - (unsigned long)(&((type *)0)->member)))

/**
 * klist_for_each_entry  -   iterate over klist of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your klist.
 * @member: the name of the klist_struct within the struct.
 */
#define klist_for_each_entry(pos, head, member)              \
    for (pos = klist_entry((head)->next, typeof(*pos), member);  \
         &pos->member != (head);    \
         pos = klist_entry(pos->member.next, typeof(*pos), member))
         
/**
 * klist_first_entry - get the first element from a klist
 * @ptr:    the klist head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the klist_struct within the struct.
 *
 * Note, that klist is expected to be not empty.
 */
#define klist_first_entry(ptr, type, member) \
    klist_entry((ptr)->next, type, member)
            
static inline void __klist_del(struct klist_head *prev, struct klist_head *next);
void klist_del(struct klist_head *entry);
void klist_add(struct klist_head *head, struct klist_head *node);
int klist_empty(const struct klist_head *head);
#endif