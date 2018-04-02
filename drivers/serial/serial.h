#ifndef __SERIAL_H__
#define __SERIAL_H__
#include "../global/include.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BAUD_RATE_2400                  2400
#define BAUD_RATE_4800                  4800
#define BAUD_RATE_9600                  9600
#define BAUD_RATE_19200                 19200
#define BAUD_RATE_38400                 38400
#define BAUD_RATE_57600                 57600
#define BAUD_RATE_115200                115200
#define BAUD_RATE_230400                230400
#define BAUD_RATE_460800                460800
#define BAUD_RATE_921600                921600
#define BAUD_RATE_2000000               2000000
#define BAUD_RATE_3000000               3000000

#define DATA_BITS_5                     5
#define DATA_BITS_6                     6
#define DATA_BITS_7                     7
#define DATA_BITS_8                     8
#define DATA_BITS_9                     9

#define STOP_BITS_1                     0
#define STOP_BITS_2                     1
#define STOP_BITS_3                     2
#define STOP_BITS_4                     3

#define PARITY_NONE                     0
#define PARITY_ODD                      1
#define PARITY_EVEN                     2

#ifndef SERIAL_RB_BUFSZ
#define SERIAL_RB_BUFSZ              64
#endif

/* Default config for serial_attr structure */
#define SERIAL_CONFIG_DEFAULT           \
{                                          \
    BAUD_RATE_115200, /* 115200 bits/s */  \
    DATA_BITS_8,      /* 8 databits */     \
    STOP_BITS_1,      /* 1 stopbit */      \
    PARITY_NONE,      /* No parity  */     \
}



#define SERIAL_EVENT_INT_RX     0x01
#define SERIAL_EVENT_DMA_RX     0x02
#define SERIAL_EVENT_INT_TX     0x03
#define SERIAL_EVENT_DMA_TX     0x04


#define SERIAL_FLAG_INT_TX  0x0001
#define SERIAL_FLAG_INT_RX  0x0002
#define SERIAL_FLAG_DMA_TX  0x0004
#define SERIAL_FLAG_DMA_RX  0x0008
#define SERIAL_FLAG_INIT    0x0010

#define SERIAL_CONFIG_INT_RX    0x01
#define SERIAL_CONFIG_INT_TX    0x02
#define SERIAL_CONFIG_DMA_RX    0x03
#define SERIAL_CONFIG_DMA_TX    0x04

#define SERIAL_CONFIG_SET       0x01
#define SERIAL_CONFIG_RESET     0x02

#define SERIAL_DMA_DIR_TX       0x01
#define SERIAL_DMA_DIR_RX       0x02


#define SERIAL_F_SETFL      0x01
#define SERIAL_F_GETFL      0x02



struct serial_attr{
    kl_u32_t baud_rate;
    kl_u32_t data_bits        :4;
    kl_u32_t stop_bits        :2;
    kl_u32_t parity           :2;
};

struct serial_fifo{
    kl_u8_t *buffer;
    int put_index,get_index;
    int bufsz;
};

struct serial{
    struct kernel_device parent;
    int flags;
    const struct serial_operations *ops;
    struct serial_attr attr;
    void *serial_rx;
    void *serial_tx;
    void *private_data;
};

struct serial_operations{
    int (*init)(struct serial *serial);
    int (*fputc)(struct serial *serial, int ch);
    int (*fgetc)(struct serial *serial);
    int (*config)(struct serial *serial, int cmd, void * arg);
};


/**
  * @brief This function is used to receive data from interrupt.
  * @param serial: instance of serial.
  * @param data: pointer to data to be received
  * @param length: length of data to be received.
  * @retval length has been received.
  */
static int _serial_int_rx(struct serial *serial, kl_u8_t *data, kl_size_t length);

/**
  * @brief This function is used to send data from interrupt.
  * @param serial: instance of serial.
  * @param data: pointer to data to be sent
  * @param length: length of data to be sent.
  * @retval length has been sent.
  */
static int _serial_int_tx(struct serial *serial, const kl_u8_t *data, kl_size_t length);

/**
  * @brief This function is used to send data in polling mode.
  * @param serial: instance of serial.
  * @param data: pointer to data to be sent
  * @param length: length of data to be sent.
  * @retval length has been sent.
  */
static int _serial_poll_tx(struct serial *serial, kl_u8_t *buffer, kl_size_t length);

/**
  * @brief This function is used to receive data in polling mode.
  * @param serial: instance of serial.
  * @param data: pointer to data to be received
  * @param length: length of data to be received.
  * @retval length has been received.
  */
static int _serial_poll_rx(struct serial *serial, kl_u8_t *buffer, kl_size_t length);


int serial_register(const char *name, struct serial *serial);
/**
  * @brief This function is used to respond serial interrupt.
  * @param serial: instance of serial.
  * @param event: serial interrupt event.
  * @param arg: argument.
  * @retval None.
  */
void serial_hw_isr(struct serial *serial, int event, void *arg);

#ifdef __cplusplus
}
#endif
#endif
