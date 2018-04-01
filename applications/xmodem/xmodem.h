#ifndef _XMODEM_H_
#define _XMODEM_H_
#include "../../global/include.h"


struct xmodem{
    kl_u8_t *buffer;
    kl_u16_t bufsz;
    kl_u8_t flags;
    kl_u8_t startint;
    kl_u8_t pktid;
    kl_u32_t address;
    kl_u32_t offset;
    kl_u32_t nrcvints;
    const struct xmodem_operations *ops;
};

struct xmodem_operations{
    int (*read)(struct xmodem *xmodem, kl_u8_t *buffer, kl_u16_t length);
    int (*write)(struct xmodem *xmodem, kl_u8_t *buffer, kl_u16_t length);
    int (*pipewrite)(kl_u32_t address, void *buffer, kl_u16_t length);
};

struct xmodem_packet{
    kl_u8_t header;
    kl_u8_t id;
    kl_u8_t nid;
    kl_u16_t pdlen;
    kl_u8_t *payload;
    kl_u16_t checksum;
};



/**
  * @brief This function is used to write data.
  * @param xmodem: instance of xmodem.
  * @param buffer: data buffer to be writed.
  * @param length: data length.
  * @retval ints has been writed.
  *         -1: fail.
  */
int xmodem_write(struct xmodem *xmodem, kl_u8_t *buffer, kl_u16_t length);

/**
  * @brief This function is used to read data.
  * @param xmodem: instance of xmodem.
  * @param buffer: buffer to save xmodem data.
  * @param length: data length.
  * @retval ints has been read.
  *         -1: fail.
  */
int xmodem_read(struct xmodem *xmodem, kl_u8_t *buffer, kl_u16_t length);

/**
  * @brief This function is used to send xmodem packet.
  * @param xmodem: instance of xmodem.
  * @param packet: xmodem packet to be sent.
  * @retval 0: success.
  *         -1: fail.
  */
int xmodem_send_packet(struct xmodem *xmodem, struct xmodem_packet *packet);

/**
  * @brief This function is used to receive xmodem packet.
  * @param xmodem: instance of xmodem.
  * @param packet: xmodem packet has been received.
  * @retval 0: success.
  *         -1: fail.
  */
int xmodem_recv_packet(struct xmodem *xmodem, struct xmodem_packet *packet);


/**
  * @brief This function is used to run xmodem client.
  * @param xmodem: instance of xmodem.
  * @retval 0: success.
  *         -1: fail.
  */
int xmodem_client_process(struct xmodem *xmodem);
#endif /* _XMODEM_H_ */
