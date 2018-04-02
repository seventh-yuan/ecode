/**
  *******************************************************
  *@file        device.c
  *@author      Arno
  *@version     V1.0.0
  *@date        2018/4/1
  *@brief       device  module.
  *             This is the kernel part of ecode.
  *******************************************************
  */

/* Includes----------------------------------------------*/
#include "device.h"
#if CONFIG_USING_DEVICE==1
#include <string.h>

/*Private variables--------------------------------------*/
KLIST_HEAD(kdev_head);
/*Public variables---------------------------------------*/
/*Private functions--------------------------------------*/
__kernel_inline void device_put(const char *name, kernel_device_t dev)
{
    dev->name = name;
    klist_add(&kdev_head, &dev->parent);
}

__kernel_inline kernel_device_t device_get(const char *name)
{
    struct klist_head *node;
    kernel_device_t dev;
    
    klist_for_each(node, &kdev_head)
    {
        dev = container_of(node, struct kernel_device, parent);
        if(0==strcmp(name, dev->name))
            return dev;
    }
    return KL_NULL;
}
/*Public functions---------------------------------------*/

/**
  * @brief This function is used to register device.
  * @param name: device name.
  * @param dev: dev to be register.
  * @retval 0:ok, -1:failed.
  */
int device_register(const char *name, kernel_device_t dev)
{
    if(dev->ops==KL_NULL)
        return -1;
    if(dev->ops->init!=KL_NULL)
    {
        if(dev->ops->init(dev)!=0)
            return -1;
    }
    
    device_put(name, dev);
    
    return 0;
}

/**
  * @brief This function is used to open a device.
  * @param name: device name.
  * @param flags: flags when open device.
  * @retval device instance.
  */
kernel_device_t device_open(const char *name, int flags)
{
    kernel_device_t dev = device_get(name);
    
    if(dev->ops->open!=KL_NULL)
    {
        if(dev->ops->open(dev, flags)<0)
            return KL_NULL;
    }
    
    return dev;
}

/**
  * @brief This function is used to close a device.
  * @param dev: device instance.
  * @retval 0: ok, -1:failed.
  */
int device_close(kernel_device_t dev)
{
    if(dev->ops->close!=KL_NULL)
    {
        if(dev->ops->close(dev)<0)
            return -1;
    }
    return 0;
}
/**
  * @brief This function is used to write data to device.
  * @param dev: device instance.
  * @param buf: data to write.
  * @param len: length of data to write.
  * @retval -1: fialed, others: number of bytes has been writed.
  */
int device_write(kernel_device_t dev, const kl_u8_t *buf, kl_size_t len)
{
    int ret = 0;
    
    if(dev->ops->write!=KL_NULL)
        ret = dev->ops->write(dev, buf, len);
    return ret;
}

/**
  * @brief This function is used to read data from device.
  * @param dev: device instance.
  * @param buf: destination to save data.
  * @param len: length of data to read.
  * @retval -1: fialed, others: number of bytes has been read.
  */
int device_read(kernel_device_t dev, kl_u8_t *buf, kl_size_t len)
{
    int ret = 0;
    
    if(dev->ops->read!=KL_NULL)
        ret = dev->ops->read(dev, buf, len);
    return ret;
}

/**
  * @brief This function is used to control device.
  * @param dev: device instance.
  * @param cmd: command.
  * @param arg: command argument.
  * @retval 0:ok, -1:failed.
  */
int device_control(kernel_device_t dev, int cmd, void *arg)
{
    if(dev->ops->cntl!=NULL)
    {
        if(dev->ops->cntl(dev, cmd, arg)<0)
            return -1;
    }
    return 0;
}
#endif