/**
  *******************************************************
  *@file        drv_uart.c
  *@author      Arno
  *@version     V1.0.0
  *@date        2018/4/1
  *@brief       uart driver  module.
  *******************************************************
  */
/* Includes----------------------------------------------*/
#include "drv_uart.h"
#include "stm32f3xx_ll_usart.h"
#include "stm32f3xx_ll_gpio.h"
#include "stm32f3xx_ll_rcc.h"
#include "stm32f3xx_ll_bus.h"

/*macro variables----------------------------------------*/
#define USING_UART1
#define UART1_INSTANCE           USART2
#define UART1_CLK_ENABLE()     LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2)
#define UART1_CLK_SOURCE()     LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1)
#define UART1_GPIO_CLK_ENABLE()    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)
#define UART1_TX_PIN               GPIO_PIN_2
#define UART1_TX_GPIO_PORT              GPIOA
#define UART1_SET_TX_GPIO_AF()      LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_2, LL_GPIO_AF_7)
#define UART1_RX_PIN               GPIO_PIN_3
#define UART1_RX_GPIO_PORT              GPIOA
#define UART1_SET_RX_GPIO_AF()                    LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_3, LL_GPIO_AF_7)
#define UART1_IRQ_NUM              USART2_IRQn
#define UART1_INTERRUPT              USART2_IRQHandler


/*Private variables--------------------------------------*/
/*Public variables---------------------------------------*/
/*Private functions--------------------------------------*/
/**
  * @brief uart rcc configuration.
  * @retval None
  */
static void uart_rcc_configuration(void)
{
#ifdef USING_UART1
    UART1_CLK_ENABLE();
    UART1_GPIO_CLK_ENABLE();
    UART1_CLK_SOURCE();
#endif
}
/**
  * @brief uart gpio init.
  * @retval None
  */
static void uart_gpio_init(void)
{
#ifdef USING_UART1
    /* Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up */
    LL_GPIO_SetPinMode(UART1_TX_GPIO_PORT, UART1_TX_PIN, LL_GPIO_MODE_ALTERNATE);
    UART1_SET_TX_GPIO_AF();
    LL_GPIO_SetPinSpeed(UART1_TX_GPIO_PORT, UART1_TX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(UART1_TX_GPIO_PORT, UART1_TX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(UART1_TX_GPIO_PORT, UART1_TX_PIN, LL_GPIO_PULL_UP);

    /* Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up */
    LL_GPIO_SetPinMode(UART1_RX_GPIO_PORT, UART1_RX_PIN, LL_GPIO_MODE_ALTERNATE);
    UART1_SET_RX_GPIO_AF();
    LL_GPIO_SetPinSpeed(UART1_RX_GPIO_PORT, UART1_RX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(UART1_RX_GPIO_PORT, UART1_RX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(UART1_RX_GPIO_PORT, UART1_RX_PIN, LL_GPIO_PULL_UP);
#endif
    
}

/**
  * @brief stm uart init.
  * @retval None
  */
static void stm_uart_init(void)
{
#ifdef USING_UART1
    NVIC_SetPriority(UART1_IRQ_NUM, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),10, 0));
    NVIC_EnableIRQ(UART1_IRQ_NUM);
    
    LL_USART_SetDataWidth(UART1_INSTANCE, LL_USART_DATAWIDTH_8B);
    
    LL_USART_SetStopBitsLength(UART1_INSTANCE, LL_USART_STOPBITS_1);
    
    LL_USART_SetParity(UART1_INSTANCE, LL_USART_PARITY_NONE);
    
    LL_USART_SetTransferDirection(UART1_INSTANCE, LL_USART_DIRECTION_TX_RX);
    
    LL_USART_SetHWFlowCtrl(UART1_INSTANCE, LL_USART_HWCONTROL_NONE);
    
    LL_USART_SetBaudRate(UART1_INSTANCE, SystemCoreClock/2, LL_USART_OVERSAMPLING_16, 115200);
    
    LL_USART_Enable(UART1_INSTANCE);
    
    while((!(LL_USART_IsActiveFlag_TEACK(UART1_INSTANCE))) || (!(LL_USART_IsActiveFlag_REACK(UART1_INSTANCE))))
    {
        
    }
    //LL_USART_EnableIT_RXNE(UART1_INSTANCE);
    
#endif
}

/*Public functions---------------------------------------*/
/**
  * @brief console write interface.
  * @param buf: data to write.
  * @param len: length of data to write.
  * @retval number of bytes has been writed.
  */
int console_hw_write(const kl_u8_t *buf, kl_size_t len)
{
    kl_size_t size = len;
    while(len)
    {
        while(!LL_USART_IsActiveFlag_TXE(UART1_INSTANCE));
        LL_USART_TransmitData8(UART1_INSTANCE, *buf);
        buf++;
        len--;
    }
    
    return (size-len);
}
/**
  * @brief console read interface.
  * @param buf: data to read.
  * @param len: length of data to read.
  * @retval number of bytes has been read.
  */
int console_hw_read(kl_u8_t *buf, kl_size_t len)
{
    kl_size_t size = len;
    
    while(len)
    {
        while(!LL_USART_IsActiveFlag_RXNE(UART1_INSTANCE));
        *buf ++ = LL_USART_ReceiveData8(UART1_INSTANCE);
        len --;
    }
    return (size-len);
}
/**
  * @brief uart hardware init.
  * @retval None.
  */
void uart_hw_init(void)
{
    uart_rcc_configuration();
    uart_gpio_init();
    
    stm_uart_init();
}

