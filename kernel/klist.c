/**
  *******************************************************
  *@file        klist.c
  *@author      Arno
  *@version     V1.0.0
  *@date        2018/4/1
  *@brief       kernel list module.
  *             This is the kernel part of ecode.
  *******************************************************
  */
/* Includes----------------------------------------------*/
#include "klist.h"

/*Private variables--------------------------------------*/
/*Public variables---------------------------------------*/
/*Private functions--------------------------------------*/
/*Public functions---------------------------------------*/
  
__kernel_inline void __klist_del(struct klist_head *prev, struct klist_head *next)
{
    next->prev = prev;
    prev->next = next;
}

void klist_del(struct klist_head *entry)
{
    __klist_del(entry->prev, entry->next);
    entry->prev=KL_NULL;
    entry->next=KL_NULL;
}

__kernel_inline void __klist_add(struct klist_head *entry,struct klist_head *prev, struct klist_head *next)
{
    next->prev = entry;
    entry->next = next;
    entry->prev = prev;
    prev->next = entry;
}

void klist_add(struct klist_head *head, struct klist_head *entry)
{
    __klist_add(entry, head, head->next);

}

int klist_empty(const struct klist_head *head)
{
    return (head->next==head);
}