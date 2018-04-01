#ifndef __KMEM_H__
#define __KMEM_H__
#include "../global/include.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
  * @brief This function is used to alloc memory.
  * @param size: size of memory to alloc.
  * @retval The handle of memory has been alloced.
  *         KL_NULL if no memory can be alloced.
  */
void *kmem_alloc(kl_size_t size);

/**
  * @brief This function is used to realloc memory.
  * @param ptr: old handle of memory
  * @param size: size of memory to alloc.
  * @retval The handle of memory has been realloced.
  *         KL_NULL if no memory can be realloced.
  */
void *kmem_realloc(void *ptr,kl_size_t size);

/**
  * @brief This function is used to free memory.
  * @param ptr: the handle of memory to be free.
  * @retval None
  */
void kmem_free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif