#ifndef __DEVICE_H__
#define __DEVICE_H__
#include "../global/include.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
  * @brief This function is used to open a device.
  * @param name: device name.
  * @param flags: flags when open device.
  * @retval device instance.
  */
kernel_device_t device_open(const char *name, int flags);

/**
  * @brief This function is used to close a device.
  * @param dev: device instance.
  * @retval 0: ok, -1:failed.
  */
int device_close(kernel_device_t *dev);

/**
  * @brief This function is used to write data to device.
  * @param dev: device instance.
  * @param buf: data to write.
  * @param len: length of data to write.
  * @retval -1: fialed, others: number of bytes has been writed.
  */
int device_write(kernel_device_t *dev, const kl_u8_t *buf, kl_size_t len);

/**
  * @brief This function is used to read data from device.
  * @param dev: device instance.
  * @param buf: destination to save data.
  * @param len: length of data to read.
  * @retval -1: fialed, others: number of bytes has been read.
  */
int device_read(kernel_device_t *dev, kl_u8_t *buf, kl_size_t len);

/**
  * @brief This function is used to control device.
  * @param dev: device instance.
  * @param cmd: command.
  * @param arg: command argument.
  * @retval 0:ok, -1:failed.
  */
int device_control(kernel_device_t *dev, int cmd, void *arg);
#ifdef __cplusplus
}
#endif
#endif