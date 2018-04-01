/**
  *******************************************************
  *@file        xmodem.c
  *@author      Arno
  *@version     V1.0.0
  *@date        2017/11/12
  *@brief       runtime  module.
  *             This module is xmodem procotol .
  *******************************************************
  */
  
/* Includes----------------------------------------------*/
#include "xmodem.h"
#include <string.h>
#include "../../misc/crc16.h"


/*
 * XMODEM protocol keywords
 */
#define XMODEM_SOH                    0x01
#define XMODEM_STX                    0x02
#define XMODEM_EOT                    0x04
#define XMODEM_ACK                    0x06
#define XMODEM_NAK                    0x15
#define XMODEM_CAN                    0x18
#define XMODEM_CTRLZ                0x1A

#define XMODEM_START                'C'

#define XMODEM_RETRY_TIMES    32

#define XMODEM_FLAG_START   0x01
#define XMODEM_FLAG_RECV    0x02
#define XMODEM_FLAG_EOT     0x04


/**
  * @brief This function is used to get xmodem int.
  * @param xmodem: instance of xmodem.
  * @retval data xmodem get.
  *         -1: fail.
  */
static int xmodem_getc(struct xmodem *xmodem)
{
    int ch = 0;
    
    const struct xmodem_operations *ops = xmodem->ops;
    
    kernel_msec_t last_time = kernel_get_millis();
    
    
    while (1)
    {
        if(ops->read(xmodem, (kl_u8_t *)&ch, 1)<=0)
        {
            if(kernel_get_millis()-last_time>=999)
                return -1;
            continue;
        }
        else
            break;
    }

    return ch;
}

/**
  * @brief This function is used to put xmodem int.
  * @param xmodem: instance of xmodem.
  * @param ch: data to be put.
  * @retval 0: success.
  *         -1: fail.
  */
static int xmodem_putc(struct xmodem *xmodem, int ch)
{
    const struct xmodem_operations *ops = xmodem->ops;
    
    
    return ops->write(xmodem,(kl_u8_t *)&ch,1);
}

/**
  * @brief This function is used to check packet.
  * @param bcrc16: 0 use crc16.
  * @param         1 not use crc16.
  * @param packet: xmodem packet.
  * @retval 1: success.
  *         0: fail.
  */
static int xmodem_checksum(kl_s8_t bcrc16, struct xmodem_packet *packet)
{
    if(bcrc16)
    {
        kl_u16_t crc16 = crc16_ccitt((kl_u8_t*)packet->payload, packet->pdlen);
        
        if(crc16 != packet->checksum)
        {
            return 0;
        }
    }
    else
    {
        kl_u16_t icount;
        kl_u16_t checksum;
        
        for(icount = 0; icount < packet->pdlen; ++icount)
        {
            checksum += *(packet->payload + icount);
        }

        if(checksum != packet->checksum )
        {
            return 0;
        }
    }

    return 1;
}

/**
  * @brief This function is used to write data.
  * @param xmodem: instance of xmodem.
  * @param buffer: data buffer to be writed.
  * @param length: data length.
  * @retval ints has been writed.
  *         -1: fail.
  */
int xmodem_write(struct xmodem *xmodem, kl_u8_t *buffer, kl_u16_t length)
{

    const struct xmodem_operations *ops = xmodem->ops;

    return ops->write(xmodem, buffer, length);
}

/**
  * @brief This function is used to read data.
  * @param xmodem: instance of xmodem.
  * @param buffer: buffer to save xmodem data.
  * @param length: data length.
  * @retval ints has been read.
  *         -1: fail.
  */
int xmodem_read(struct xmodem *xmodem, kl_u8_t *buffer, kl_u16_t length)
{
    const struct xmodem_operations *ops = xmodem->ops;

    return ops->read(xmodem, buffer, length);
}

/**
  * @brief This function is used to send xmodem packet.
  * @param xmodem: instance of xmodem.
  * @param packet: xmodem packet to be sent.
  * @retval 0: success.
  *         -1: fail.
  */
int xmodem_send_packet(struct xmodem *xmodem, struct xmodem_packet *packet)
{
    
    if(xmodem_putc(xmodem, packet->header)<0)
        return -1;
    if(xmodem_putc(xmodem, packet->id)<0)
        return -1;
    if(xmodem_putc(xmodem, packet->nid)<0)
        return -1;
    
    if(xmodem_write(xmodem, packet->payload, packet->pdlen)<0)
        return -1;
    if(xmodem_putc(xmodem, packet->checksum)<0)
        return -1;
    
    return 0;
}


/**
  * @brief This function is used to receive xmodem packet.
  * @param xmodem: instance of xmodem.
  * @param packet: xmodem packet has been received.
  * @retval 0: success.
  *         -1: fail.
  */
int xmodem_recv_packet(struct xmodem *xmodem, struct xmodem_packet *packet)
{
    kl_u16_t iretry = 0;
    int rcvint;
    kl_s8_t busecrc = 1;
    
    memset(packet->payload, 0, packet->pdlen);
    
    for(iretry=0;iretry<XMODEM_RETRY_TIMES;iretry++)
    {
        if(xmodem->flags&XMODEM_FLAG_START)
        {
            xmodem_putc(xmodem, xmodem_putc(xmodem,xmodem->startint));
        }
        rcvint = xmodem_getc(xmodem);
        if(rcvint<0)
            continue;
        
        switch(rcvint)
        {
        case XMODEM_SOH:
            packet->pdlen = 128;
            goto xmodem_receive;
        case XMODEM_STX:
            packet->pdlen = 1024;
            goto xmodem_receive;
        case XMODEM_EOT:
            xmodem_putc(xmodem, XMODEM_ACK);
            xmodem->flags = XMODEM_EOT;
            return 0;
        case XMODEM_CAN:
            rcvint = xmodem_getc(xmodem);
            if(rcvint == XMODEM_CAN)
            {
                xmodem_putc(xmodem, XMODEM_ACK);
                return -1;
            }
            break;
        default:
            break;
        }
        
    }
    
    xmodem_putc(xmodem, XMODEM_CAN);
    xmodem_putc(xmodem, XMODEM_CAN);
    xmodem_putc(xmodem, XMODEM_CAN);
    return -1;
xmodem_receive:
    xmodem->flags = XMODEM_FLAG_RECV;
    packet->header = rcvint;
    
    rcvint = xmodem_getc(xmodem);
    if(rcvint<0)
        goto xmodem_reject;
    packet->id = rcvint;
    
    rcvint = xmodem_getc(xmodem);
    if(rcvint<0)
        goto xmodem_reject;
    packet->nid = rcvint;
    
    for(kl_u16_t i=0;i<packet->pdlen;i++)
    {
        rcvint = xmodem_getc(xmodem);
        if(rcvint<0)
            goto xmodem_reject;
        packet->payload[i]=rcvint;
    }
    
    if(busecrc)
    {
        kl_u16_t temp = 0;
        rcvint = xmodem_getc(xmodem);
        if(rcvint<0)
            goto xmodem_reject;
        temp = (rcvint<<8);
        rcvint = xmodem_getc(xmodem);
        if(rcvint<0)
            goto xmodem_reject;
        temp |= rcvint;
        packet->checksum = temp;
    }
    else
    {
        rcvint = xmodem_getc(xmodem);
        if(rcvint<0)
            goto xmodem_reject;
        packet->checksum = rcvint;
    }
    if((packet->id==(kl_u8_t)~packet->nid)&&
       ((packet->id==xmodem->pktid)||(packet->id==xmodem->pktid-1))&&
        (xmodem_checksum(busecrc, packet)))
    {
        if(packet->id==xmodem->pktid)
        {
            if(xmodem->ops->pipewrite==NULL)
                return -1;
            
            if(xmodem->ops->pipewrite(xmodem->address+xmodem->offset, packet->payload, packet->pdlen)<0)
                return -1;
            xmodem->offset += packet->pdlen;
            xmodem->nrcvints += packet->pdlen;
            
            ++xmodem->pktid;
        }
    }
    xmodem_putc(xmodem, XMODEM_ACK);
    return 0;
    
xmodem_reject:
    xmodem_putc(xmodem, XMODEM_NAK);
    return 0;
}


/**
  * @brief This function is used to run xmodem client.
  * @param xmodem: instance of xmodem.
  * @retval 0: success.
  *         -1: fail.
  */
int xmodem_client_process(struct xmodem *xmodem)
{
    if(xmodem->buffer==NULL)
    {
        print_error("xmodem buffer should not be NULL.");
        return -1;
    }
    
    xmodem->flags = XMODEM_FLAG_START;
    xmodem->startint = XMODEM_START;
    xmodem->pktid = 1;
    xmodem->nrcvints = 0;
    xmodem->offset = 0;
    struct xmodem_packet xmodem_packet;

    while(1)
    {
        memset(&xmodem_packet, 0, sizeof(struct xmodem_packet));
        xmodem_packet.payload = xmodem->buffer;
    
        if(xmodem_recv_packet(xmodem, &xmodem_packet)<0)
            return -1;
        if(xmodem->flags&XMODEM_FLAG_EOT)
            break;
    }
    return 0;
}
