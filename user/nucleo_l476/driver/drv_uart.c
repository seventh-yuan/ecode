#include "drv_uart.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_ll_usart.h"
#include "serial/serial.h"

//#define USE_UART1

#define UART1_INSTANCE           USART1
#define UART1_CLK_ENABLE()     __HAL_RCC_USART1_CLK_ENABLE()
#define UART1_CLK_SOURCE()     //LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1)
#define UART1_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()
#define UART1_TX_PIN               GPIO_PIN_9
#define UART1_TX_GPIO_PORT              GPIOA
#define UART1_RX_PIN               GPIO_PIN_10
#define UART1_RX_GPIO_PORT              GPIOA
#define UART1_AF_CFG                    GPIO_AF7_USART1
#define UART1_IRQ_NUM              USART1_IRQn
#define UART1_INTERRUPT              USART1_IRQHandler

#define USE_UART2

#define UART2_INSTANCE           USART2
#define UART2_CLK_ENABLE()     __HAL_RCC_USART2_CLK_ENABLE()
#define UART2_CLK_SOURCE()     //LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1)
#define UART2_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()
#define UART2_TX_PIN               GPIO_PIN_2
#define UART2_TX_GPIO_PORT              GPIOA
#define UART2_RX_PIN               GPIO_PIN_3
#define UART2_RX_GPIO_PORT              GPIOA
#define UART2_AF_CFG                    GPIO_AF7_USART2
#define UART2_IRQ_NUM              USART2_IRQn
#define UART2_INTERRUPT              USART2_IRQHandler

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
    //LL_USART_ConfigCharacter(UART1_INSTANCE, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);

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

#ifdef USE_UART1
struct stm_uart uart1 = {
    .instance = UART1_INSTANCE,
    .irq = UART1_IRQ_NUM,
};

uint8_t serial1_rx_buffer[SERIAL_RB_BUFSZ];

struct serial_fifo serial1_rx_fifo={
    .buffer = serial1_rx_buffer,
    .put_index = 0,
    .get_index = 0,
    .bufsz = SERIAL_RB_BUFSZ,
};

static struct serial serial1={
    .flags = SERIAL_FLAG_INT_RX,
    .ops = &stm_uart_ops,
    .attr = SERIAL_CONFIG_DEFAULT,
    
    .serial_rx = &serial1_rx_fifo,
    
    .private_data = &uart1,
};

void UART1_INTERRUPT(void)
{
    uart_isr(&serial1);
}
#endif


#ifdef USE_UART2
struct stm_uart uart2 = {
    .instance = UART2_INSTANCE,
    .irq = UART2_IRQ_NUM,
};

uint8_t serial2_rx_buffer[SERIAL_RB_BUFSZ];

struct serial_fifo serial2_rx_fifo={
    .buffer = serial2_rx_buffer,
    .put_index = 0,
    .get_index = 0,
    .bufsz = SERIAL_RB_BUFSZ,
};

static struct serial serial2={
    .flags = SERIAL_FLAG_INT_RX,
    .ops = &stm_uart_ops,
    .attr = SERIAL_CONFIG_DEFAULT,
    
    .serial_rx = &serial2_rx_fifo,
    
    .private_data = &uart2,
};

void UART2_INTERRUPT(void)
{
    uart_isr(&serial2);
}
#endif

static void uart_rcc_configuration(void)
{
#ifdef USE_UART1
    UART1_CLK_ENABLE();
    UART1_CLK_SOURCE();
    UART1_GPIO_CLK_ENABLE();
#endif
#ifdef USE_UART2
    UART2_CLK_ENABLE();
    UART2_CLK_SOURCE();
    UART2_GPIO_CLK_ENABLE();
#endif
}

static void uart_gpio_configuration(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
#ifdef USE_UART1
  
    /*##-2- Configure peripheral GPIO ##########################################*/
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = UART1_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = UART1_AF_CFG;

    HAL_GPIO_Init(UART1_TX_GPIO_PORT, &GPIO_InitStruct);

    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin = UART1_RX_PIN;
    //GPIO_InitStruct.Mode    = GPIO_MODE_INPUT;
    //GPIO_InitStruct.Pull    = GPIO_NOPULL;
    HAL_GPIO_Init(UART1_RX_GPIO_PORT, &GPIO_InitStruct);
#endif

#ifdef USE_UART2
  
    /*##-2- Configure peripheral GPIO ##########################################*/
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = UART2_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = UART2_AF_CFG;

    HAL_GPIO_Init(UART2_TX_GPIO_PORT, &GPIO_InitStruct);

    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin = UART2_RX_PIN;
    //GPIO_InitStruct.Mode    = GPIO_MODE_INPUT;
    //GPIO_InitStruct.Pull    = GPIO_NOPULL;
    HAL_GPIO_Init(UART2_RX_GPIO_PORT, &GPIO_InitStruct);
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

void uart_hw_init(void)
{
    struct stm_uart *uart;
    struct serial_attr attr = SERIAL_CONFIG_DEFAULT;

    uart_rcc_configuration();
    uart_gpio_configuration();
    
#ifdef USE_UART1
    uart = &uart1;
    
    serial1.ops = &stm_uart_ops;
    serial1.attr = attr;
    
    uart_nvic_configuration(uart);
    
    serial_register("COM1",
                    &serial1);
    
#endif

#ifdef USE_UART2
    uart = &uart2;
    
    serial2.ops = &stm_uart_ops;
    serial2.attr = attr;
    
    uart_nvic_configuration(uart);
    
    serial_register("COM2",
                    &serial2);
    
#endif
}