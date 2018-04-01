#ifndef _CRC8_H
#define _CRC8_H
#include "../global/include.h"

#ifdef __cplusplus
extern "C"{
#endif


/**
  * @brief This function is used to calculate crc8.
  * @param buffer: data buffer to calculate crc8.
  * @param len: length of data to calculate crc8.
  * @retval crc8.
  */
kl_u8_t crc8_ccitt(kl_u8_t *buffer, kl_u16_t len);

/**
  * @brief This function is used to check crc8.
  * @param buffer: data buffer to check crc8.
  * @param len: length of data to check crc8.
  * @retval 1: success.
  *         0: fail.
  */
int crc8_check(kl_u8_t tcrc,kl_u8_t *buffer, kl_u16_t len);

#ifdef __cplusplus
}
#endif
#endif
