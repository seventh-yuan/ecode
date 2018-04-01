#ifndef _FIFO_H_
#define _FIFO_H_
#include "../global/include.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
    kl_u8_t* buffer;
    kl_u16_t ifront;
    kl_u16_t irear;
    kl_u16_t nsize;
    kl_u16_t ncapacity;
}fifo_t;

/**
  * @brief This function is used to init a fifo.
  * @param fifo: instance of a fifo.
  * @param buffer: fifo buffer.
  * @param ncapacity: fifo capacity.
  * @retval 0: success.
  *         -1: fail.
  */
int fifo_init(fifo_t *fifo,kl_u8_t *buffer,kl_u16_t ncapacity);

/**
  * @brief This function is used to judge fifo empty.
  * @param fifo: instance of a fifo.
  * @retval 0: not empty.
  *         1: empty.
  */
int fifo_is_empty(fifo_t* fifo);

/**
  * @brief This function is used to judge fifo full.
  * @param fifo: instance of a fifo.
  * @retval 0: not full.
  *         1: full.
  */
int fifo_is_full(fifo_t* fifo);

/**
  * @brief This function is used to push an item to fifo.
  * @param fifo: instance of a fifo.
  * @param item: item to be pushed.
  * @retval None
  */
void fifo_push(fifo_t* fifo, kl_u8_t item);

/**
  * @brief This function is used to pop an item from fifo.
  * @param fifo: instance of a fifo.
  * @retval item to be poped.
  */
kl_u8_t fifo_pop(fifo_t* fifo);

/**
  * @brief This function is used to query count of item being pushed.
  * @param fifo: instance of a fifo.
  * @retval count of item being pushed.
  */
kl_u16_t fifo_query_pushed(fifo_t *fifo);

/**
  * @brief This function is used to reset a fifo.
  * @param fifo: instance of a fifo.
  * @retval None.
  */
void fifo_reset(fifo_t *fifo);
#ifdef __cplusplus
}
#endif

#endif /* _FIFO_H_ */
