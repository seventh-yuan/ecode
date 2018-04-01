/**
  *******************************************************
  *@file        fifo.c
  *@author      Arno
  *@version     V1.0.0
  *@date        2017/11/3
  *@brief       This module is used to provide fifo 
  *             function.
  *******************************************************
  */
  
/* Includes----------------------------------------------*/
#include "fifo.h"

/**
  * @brief This function is used to init a fifo.
  * @param fifo: instance of a fifo.
  * @param buffer: fifo buffer.
  * @param ncapacity: fifo capacity.
  * @retval 0: success.
  *         -1: fail.
  */
int fifo_init(fifo_t *fifo,kl_u8_t *buffer,kl_u16_t ncapacity)
{
    if(buffer == NULL||fifo==NULL)
        return -1;


    fifo->buffer = buffer;
    fifo->nsize = 0;
    fifo->ncapacity = ncapacity;
    fifo->ifront = 0;
    fifo->irear = 0;

    return 0;
}

/**
  * @brief This function is used to judge fifo empty.
  * @param fifo: instance of a fifo.
  * @retval 0: not empty.
  *         1: empty.
  */
int fifo_is_empty(fifo_t* fifo)
{
    if(fifo == NULL)
        return 0;
    if((fifo->ifront == fifo->irear) && (fifo->nsize == 0))
        return 1;
    return 0;
}

/**
  * @brief This function is used to judge fifo full.
  * @param fifo: instance of a fifo.
  * @retval 0: not full.
  *         1: full.
  */
int fifo_is_full(fifo_t* fifo)
{
    if(fifo == NULL)
        return 1;
    if((fifo->ifront == fifo->irear) && (fifo->nsize == fifo->ncapacity))
        return 1;
    return 0;
}

/**
  * @brief This function is used to push an item to fifo.
  * @param fifo: instance of a fifo.
  * @param item: item to be pushed.
  * @retval None
  */
void fifo_push(fifo_t* fifo, kl_u8_t item)
{
    *(fifo->buffer + fifo->irear) = item;
    if((fifo->irear + 1) == fifo->ncapacity)
    {
        fifo->irear = 0;
    }else{
        fifo->irear++;
    }
    fifo->nsize++;
}

/**
  * @brief This function is used to pop an item from fifo.
  * @param fifo: instance of a fifo.
  * @retval item to be poped.
  */
kl_u8_t fifo_pop(fifo_t* fifo)
{
    kl_u8_t item;
    item = *(fifo->buffer + fifo->ifront);
    if((fifo->ifront + 1) == fifo->ncapacity)
    {
        fifo->ifront = 0;
    }else{
        fifo->ifront++;
    }
    fifo->nsize--;

    return item;
}

/**
  * @brief This function is used to query count of item being pushed.
  * @param fifo: instance of a fifo.
  * @retval count of item being pushed.
  */
kl_u16_t fifo_query_pushed(fifo_t *fifo)
{
    return fifo->nsize;
}

/**
  * @brief This function is used to reset a fifo.
  * @param fifo: instance of a fifo.
  * @retval None.
  */
void fifo_reset(fifo_t *fifo)
{
    fifo->nsize = 0;
    fifo->ifront = 0;
    fifo->irear = 0;   
}
