#ifndef _CRC16_H
#define _CRC16_H
#include "../global/include.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
  * @brief This function is used to calculate crc16.
  * @param buffer: data buffer to calculate crc16.
  * @param len: length of data to calculate crc16.
  * @retval crc8.
  */
kl_u16_t crc16_ccitt(const kl_u8_t *buf, kl_u16_t len);

/**
  * @brief This function is used to check crc16.
  * @param buffer: data buffer to check crc16.
  * @param len: length of data to check crc16.
  * @retval 1: success.
  *         0: fail.
  */
int crc16_check(kl_u16_t tcrc,const kl_u8_t *buf, int len);

#ifdef __cplusplus
}
#endif
#endif


