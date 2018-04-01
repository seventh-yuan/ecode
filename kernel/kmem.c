/**
  *******************************************************
  *@file        kmem.c
  *@author      Arno
  *@version     V1.0.0
  *@date        2018/4/1
  *@brief       kernel memory module.
  *             This is the kernel part of ecode.
  *******************************************************
  */
  
/* Includes----------------------------------------------*/
#include "kmem.h"
#include <string.h>

/*Private variables--------------------------------------*/
/*Public variables---------------------------------------*/
/*Private functions--------------------------------------*/
/*Public functions---------------------------------------*/

/**
  * @brief This function is used to alloc memory.
  * @param size: size of memory to alloc.
  * @retval The handle of memory has been alloced.
  *         KL_NULL if no memory can be alloced.
  */
void *kmem_alloc(kl_size_t size)
{
#if CONFIG_USING_RTOS==1
    return pvPortMalloc(size);
#else
    return KL_NULL;
#endif
}

/**
  * @brief This function is used to realloc memory.
  * @param ptr: old handle of memory
  * @param size: size of memory to alloc.
  * @retval The handle of memory has been realloced.
  *         KL_NULL if no memory can be realloced.
  */
void *kmem_realloc(void *ptr,kl_size_t size)
{
    void *new = NULL;
#if CONFIG_USING_RTOS==1
    new = pvPortMalloc(size);
    if(ptr)
    {
        memcpy(new, ptr, size);
        vPortFree(ptr);
    }
#endif
    return new;
}

/**
  * @brief This function is used to free memory.
  * @param ptr: the handle of memory to be free.
  * @retval None
  */
void kmem_free(void *ptr)
{
#if CONFIG_USING_RTOS==1
    vPortFree(ptr);
#endif
}