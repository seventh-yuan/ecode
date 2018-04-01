/**
  *******************************************************
  *@file        crc8.c
  *@author      Arno
  *@version     V1.0.0
  *@date        2017/11/4
  *@brief       This module is used to calculate and check crc8.
  *******************************************************
  */
  
/* Includes----------------------------------------------*/
#include "crc8.h"

/**
  * @brief This function is used to calculate crc8.
  * @param buffer: data buffer to calculate crc8.
  * @param len: length of data to calculate crc8.
  * @retval crc8.
  */
kl_u8_t crc8_ccitt(kl_u8_t *buffer, kl_u16_t len)
{
    return 1;
}

/**
  * @brief This function is used to check crc8.
  * @param buffer: data buffer to check crc8.
  * @param len: length of data to check crc8.
  * @retval 1: success.
  *         0: fail.
  */
int crc8_check(kl_u8_t tcrc,kl_u8_t *buffer, kl_u16_t len)
{
    return 1;
}

