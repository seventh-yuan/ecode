#include "drv_uart.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_ll_usart.h"
#include "serial/serial.h"

#define USE_UART3

#define UART3_INSTANCE           USART3
#define UART3_CLK_ENABLE()     __HAL_RCC_USART3_CLK_ENABLE()
#define UART3_CLK_SOURCE()     //LL_RCC_SetUSARTClockSource(LL_RCC_USART3_CLKSOURCE_PCLK1)
#define UART3_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE()
#define UART3_TX_PIN               GPIO_PIN_8
#define UART3_TX_GPIO_PORT              GPIOD
#define UART3_RX_PIN               GPIO_PIN_9
#define UART3_RX_GPIO_PORT              GPIOD
#define UART3_AF_CFG                    GPIO_AF7_USART3
#define UART3_IRQ_NUM              USART3_IRQn
#define UART3_INTERRUPT              USART3_IRQHandler


#define UART_ENABLE_IRQ(n)            NVIC_EnableIRQ((n))
#define UART_DISABLE_IRQ(n)           NVIC_DisableIRQ((n))

struct stm_uart{
    USART_TypeDef *instance;
    UART_HandleTypeDef uart_handle;
    IRQn_Type irq;
};



static int stm_uart_init(struct serial *serial)
{
    struct stm_uart *uart;
    struct serial_attr *attr;
    UART_HandleTypeDef *huart;
    
    attr = &serial->attr;
    
    uart = (struct stm_uart *)serial->private_data;
    huart = &uart->uart_handle;
    huart->Instance = uart->instance;
    
    if(attr->data_bits==DATA_BITS_8)
    {
        huart->Init.WordLength = UART_WORDLENGTH_8B;
    }else if(attr->data_bits==DATA_BITS_9){
        huart->Init.WordLength = UART_WORDLENGTH_9B;
    }
    
    if(attr->stop_bits == STOP_BITS_1){
        huart->Init.StopBits = UART_STOPBITS_1;
    }else if(attr->stop_bits==STOP_BITS_2){
        huart->Init.StopBits = UART_STOPBITS_2;
    }
    
    if(attr->parity==PARITY_NONE)
    {
        huart->Init.Parity = UART_PARITY_NONE;
    }else if(attr->parity==PARITY_ODD){
        huart->Init.Parity = UART_PARITY_ODD;
    }else if(attr->parity==PARITY_EVEN){
        huart->Init.Parity = UART_PARITY_EVEN;
    }
        
    /* TX/RX direction */
    huart->Init.Mode = UART_MODE_TX_RX;
    
    huart->Init.OverSampling = UART_OVERSAMPLING_16;

    /* 8 data bit, 1 start bit, 1 stop bit, no parity */
    //LL_USART_ConfigCharacter(UART3_INSTANCE, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);

    /* No Hardware Flow control */
    /* Reset value is LL_USART_HWCONTROL_NONE */
    huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;

    huart->Init.BaudRate = attr->baud_rate;

    if(HAL_UART_Init(huart)!=HAL_OK)
    {
        print_error("uart hal driver init failed!");
        return -1;
    }
    
    __HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);
    __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_RXNE);
    __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_TC);
    
    return 0;
}

static void uart_isr(struct serial *serial)
{
    struct stm_uart * uart= (struct stm_uart *)serial->private_data;
    
    if(__HAL_UART_GET_FLAG(&uart->uart_handle,UART_FLAG_RXNE))
    {
        serial_hw_isr(serial, SERIAL_EVENT_INT_RX,0);
        __HAL_UART_CLEAR_FLAG(&uart->uart_handle, UART_FLAG_RXNE);
    }
}


static int stm_putc(struct serial *serial, int ch)
{
    struct stm_uart *uart;

    uart = serial->private_data;
    
    while(!LL_USART_IsActiveFlag_TXE(uart->instance));
    LL_USART_TransmitData8(uart->instance, ch);
    
    return 1;
}

static int stm_getc(struct serial *serial)
{
    int ch;
    struct stm_uart *uart;

    uart = (struct stm_uart *)serial->private_data;

    ch = -1;
    
    if(LL_USART_IsActiveFlag_RXNE(uart->instance))
    {
        ch = LL_USART_ReceiveData8(uart->instance);
    }

    return ch;
}

static const struct serial_operations stm_uart_ops={
    .init = stm_uart_init,
    .fputc = stm_putc,
    .fgetc = stm_getc,
};

#ifdef USE_UART3
struct stm_uart uart3 = {
    .instance = UART3_INSTANCE,
    .irq = UART3_IRQ_NUM,
};

uint8_t serial3_rx_buffer[SERIAL_RB_BUFSZ];

struct serial_fifo serial3_rx_fifo={
    .buffer = serial3_rx_buffer,
    .put_index = 0,
    .get_index = 0,
    .bufsz = SERIAL_RB_BUFSZ,
};

static struct serial serial3={
    .flags = SERIAL_FLAG_INT_RX,
    .ops = &stm_uart_ops,
    .attr = SERIAL_CONFIG_DEFAULT,
    
    .serial_rx = &serial3_rx_fifo,
    
    .private_data = &uart3,
};

void UART3_INTERRUPT(void)
{
    uart_isr(&serial3);
}
#endif


static void uart_rcc_configuration(void)
{
#ifdef USE_UART3
    UART3_CLK_ENABLE();
    UART3_CLK_SOURCE();
    UART3_GPIO_CLK_ENABLE();
#endif

}

static void uart_gpio_configuration(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
#ifdef USE_UART3
  
    /*##-2- Configure peripheral GPIO ##########################################*/
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = UART3_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = UART3_AF_CFG;

    HAL_GPIO_Init(UART3_TX_GPIO_PORT, &GPIO_InitStruct);

    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin = UART3_RX_PIN;
    //GPIO_InitStruct.Mode    = GPIO_MODE_INPUT;
    //GPIO_InitStruct.Pull    = GPIO_NOPULL;
    HAL_GPIO_Init(UART3_RX_GPIO_PORT, &GPIO_InitStruct);
#endif

}

static void uart_nvic_configuration(struct stm_uart *uart)
{
    NVIC_SetPriority(uart->irq, 0);
    NVIC_EnableIRQ(uart->irq);
}

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
        while(!LL_USART_IsActiveFlag_TXE(UART3_INSTANCE));
        LL_USART_TransmitData8(UART3_INSTANCE, *buf);
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
        while(!LL_USART_IsActiveFlag_RXNE(UART3_INSTANCE));
        *buf ++ = LL_USART_ReceiveData8(UART3_INSTANCE);
        len --;
    }
    return (size-len);
}

void uart_hw_init(void)
{
    struct stm_uart *uart;
    struct serial_attr attr = SERIAL_CONFIG_DEFAULT;

    uart_rcc_configuration();
    uart_gpio_configuration();
    
#ifdef USE_UART3
    uart = &uart3;
    
    serial3.ops = &stm_uart_ops;
    serial3.attr = attr;
    
    uart_nvic_configuration(uart);
    
    serial_register("COM3",
                    &serial3);
    
#endif
}