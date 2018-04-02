/**
  *******************************************************
  *@file        serial.c
  *@author      Arno
  *@version     V1.0.0
  *@date        2017/11/5
  *@brief       This module is used to provide seiral_attr
  *             function.
  *******************************************************
  */
  
/* Includes----------------------------------------------*/
#include "serial.h"

/**
  * @brief This function is used to receive data from interrupt.
  * @param serial: instance of serial.
  * @param data: pointer to data to be received
  * @param length: length of data to be received.
  * @retval length has been received.
  */
static int _serial_int_rx(struct serial *serial, kl_u8_t *data, kl_size_t length)
{
    int size;
    struct serial_fifo *rx_fifo;
     
    size = length;
    
    rx_fifo = (struct serial_fifo*) serial->serial_rx;

    while(length)
    {
        int ch;
        
        if(rx_fifo->get_index!=rx_fifo->put_index)
        {
            ch = rx_fifo->buffer[rx_fifo->get_index];
            rx_fifo->get_index += 1;
            if(rx_fifo->get_index >= rx_fifo->bufsz)
                rx_fifo->get_index = 0;
        }
        else
        {
            break;
        }
        
        *data = ch &0xff;
        data ++;
        length --;
                
    }
    
    return size - length;
}

/**
  * @brief This function is used to send data from interrupt.
  * @param serial: instance of serial.
  * @param data: pointer to data to be sent
  * @param length: length of data to be sent.
  * @retval length has been sent.
  */
static int _serial_int_tx(struct serial *serial, const kl_u8_t *data, kl_size_t length)
{
    int size;
    struct serial_fifo *tx_fifo;
    
    size = length;
    tx_fifo = (struct serial_fifo *) serial->serial_tx;

    while(length)
    {
        if(tx_fifo->put_index!=tx_fifo->get_index)
        {
            tx_fifo->buffer[tx_fifo->put_index] = *data;
            tx_fifo->put_index++;
            if(tx_fifo->put_index>=tx_fifo->bufsz)
                tx_fifo->put_index = 0;
        }
        data ++;
        length --;
    }
    
    return size-length;
}

/**
  * @brief This function is used to send data in polling mode.
  * @param serial: instance of serial.
  * @param data: pointer to data to be sent
  * @param length: length of data to be sent.
  * @retval length has been sent.
  */
static int _serial_poll_tx(struct serial *serial, kl_u8_t *buffer, kl_size_t length)
{
    int size;
    kl_u8_t *data = buffer;
    const struct serial_operations *ops = serial->ops;
    
    size = length;
    
    while(length)
    {
        if(ops->fputc(serial, *data)<0)
            break;
        data ++;
        length--;
    }
    
    return size-length;
}

/**
  * @brief This function is used to receive data in polling mode.
  * @param serial: instance of serial.
  * @param data: pointer to data to be received
  * @param length: length of data to be received.
  * @retval length has been received.
  */
static int _serial_poll_rx(struct serial *serial, kl_u8_t *buffer, kl_size_t length)
{
    int size;
    int ch = -1;
    const struct serial_operations *ops = serial->ops;
    
    size = length;
    
    while(length)
    {
        ch = ops->fgetc(serial);
        if(ch<0)
            break;
        *buffer = ch;
        buffer ++;
    }
    return size - length;
}


/**
  * @brief This function is used to open serial.
  * @param fd: serial id.
  * @retval instance of serial
  */
int serial_open(kernel_device_t dev, int flags)
{
    struct serial *serial = dev->private_data;

    if(serial->flags&SERIAL_FLAG_INIT==0)
    {
        if(serial->ops->init!=NULL)
        {
            if(serial->ops->init(serial)<0)
                return -1;
        }
    }
    
    return 0;
}

/**
  * @brief This function is used to close serial.
  * @param fd: serial id.
  * @retval instance of serial
  */
int serial_close(kernel_device_t dev)
{
    
    
    return 0;
}

/**
  * @brief This function is used to send data from serial.
  * @param fd: serial description.
  * @param buffer: pointer of data to be send
  * @param length: data length to be sent.
  * @retval length of data has been to sent.
  */
int serial_write(kernel_device_t dev,
                const kl_u8_t *buffer,
                kl_size_t length)
{
    int ret = 0;
    
    struct serial *serial = dev->private_data;
    
    if(serial->flags&SERIAL_FLAG_INT_TX)
    {
        ret = _serial_int_tx(serial, buffer, length);
    }
    else if(serial->flags&SERIAL_FLAG_DMA_TX)
    {
        
    }
    else
    {
        ret = _serial_poll_tx(serial, (kl_u8_t *)buffer, length);
    }
    
    return ret;
}

/**
  * @brief This function is used to receive data from serial.
  * @param fd: serial description.
  * @param buffer: pointer of data to be received
  * @param length: data length to be received.
  * @retval length of data has been to received.
  */
int serial_read(kernel_device_t dev, kl_u8_t *buffer, kl_size_t length)
{
    int ret = 0;
    
    struct serial *serial = dev->private_data;
    
    if(serial->flags&SERIAL_FLAG_INT_RX)
    {
        ret = _serial_int_rx(serial, (kl_u8_t *)buffer, length);
    }
    else if(serial->flags&SERIAL_FLAG_DMA_RX)
    {
        
    }
    else
    {
        ret = _serial_poll_rx(serial, (kl_u8_t *)buffer, length);
    }
    
    return ret;
}


static const struct device_operations serial_ops={
    .open = serial_open,
    .close = serial_close,
    .read = serial_read,
    .write = serial_write,
};

/**
  * @brief This function is used to register serial.
  * @param fd: serial id.
  * @param serial: instance of serial.
  * @retval 0: success.
  *         -1: fail.
  */
int serial_register(const char *name, struct serial *serial)
{
    serial->parent.private_data = serial;
    serial->parent.ops = &serial_ops;
    if(serial->ops->init!=NULL)
    {
        if(serial->ops->init(serial)<0)
            return -1;
        serial->flags |= SERIAL_FLAG_INIT;
    }
    
    if(device_register(name, &serial->parent)<0)
        return -1;
    
    return 0;
}

/**
  * @brief This function is used to respond serial interrupt.
  * @param serial: instance of serial.
  * @param event: serial interrupt event.
  * @param arg: argument.
  * @retval None.
  */
void serial_hw_isr(struct serial *serial, int event, void *arg)
{
    int ch;
    struct serial_fifo *rx_fifo;

    const struct serial_operations *ops = serial->ops;
    switch(event&0xFF)
    {
        case SERIAL_EVENT_INT_RX:
        {
            int ch = -1;
            rx_fifo = (struct serial_fifo *)serial->serial_rx;
            
            while(1)
            {
                ch = serial->ops->fgetc(serial);
                if(ch == -1)
                    break;
                rx_fifo->buffer[rx_fifo->put_index]=ch;
                rx_fifo->put_index += 1;
                if(rx_fifo->put_index>=rx_fifo->bufsz)
                    rx_fifo->put_index = 0;
                if(rx_fifo->put_index == rx_fifo->get_index)
                {
                    rx_fifo->get_index += 1;
                    if(rx_fifo->put_index >= rx_fifo->bufsz)
                    {
                        rx_fifo->get_index = 0;
                    }
                }
            }

            break;
        }
        case SERIAL_EVENT_DMA_RX:
        {

            break;
        }
        case SERIAL_EVENT_INT_TX:
        {
            struct serial_fifo *tx_fifo;
            
            tx_fifo = (struct serial_fifo *)serial->serial_tx;
            
            if(tx_fifo->get_index!=tx_fifo->put_index)
            {
                ch = tx_fifo->buffer[tx_fifo->get_index];
                if(serial->ops->fputc(serial,ch)<0)
                    break;
                tx_fifo->get_index +=1;
                if(tx_fifo->get_index>=tx_fifo->bufsz)
                    tx_fifo->get_index = 0;
            }
            else
            {
                if(serial->ops->config!=NULL)
                {
                    ops->config(serial, SERIAL_CONFIG_INT_TX, (void *)SERIAL_CONFIG_RESET);
                }
            }
            break;
        }
        case SERIAL_EVENT_DMA_TX:
        {

            break;
        }
    }
}



