#include "drv_led.h"
#include "stm32f3xx_hal.h"



struct led{
    uint16_t pin;
    uint32_t portbase;
    uint32_t mode;
};

const struct led leds[]={
    {GPIO_PIN_5,GPIOA_BASE, GPIO_MODE_OUTPUT_PP},
    {NULL,NULL,NULL},
};


static void led_set_port_clk(uint32_t portbase)
{
    switch(portbase)
    {
    case GPIOA_BASE:
      __HAL_RCC_GPIOA_CLK_ENABLE();
      break;
    case GPIOB_BASE:
      __HAL_RCC_GPIOB_CLK_ENABLE();
      break;
#if defined(GPIOC_BASE)
    case GPIOC_BASE:
      __HAL_RCC_GPIOC_CLK_ENABLE();
      break;
#endif
#if defined(GPIOD_BASE)
    case GPIOD_BASE:
      __HAL_RCC_GPIOD_CLK_ENABLE();
      break;
#endif
#if defined(GPIOE_BASE)
    case GPIOE_BASE:
      __HAL_RCC_GPIOE_CLK_ENABLE();
      break;
#endif
#if defined(GPIOF_BASE) 
    case GPIOF_BASE:
      __HAL_RCC_GPIOF_CLK_ENABLE();
      break;
#endif
#if defined(GPIOG_BASE)
    case GPIOG_BASE:
      __HAL_RCC_GPIOG_CLK_ENABLE();
      break;
#endif
#if defined(GPIOH_BASE)
    case GPIOH_BASE:
      __HAL_RCC_GPIOH_CLK_ENABLE();
      break;
#endif
#if defined(GPIOI_BASE)
    case GPIOI_BASE:
      __HAL_RCC_GPIOI_CLK_ENABLE();
      break;
#endif   
#if defined(GPIOJ_BASE)
    case GPIOJ_BASE:
      __HAL_RCC_GPIOJ_CLK_ENABLE();
      break;
#endif
#if defined(GPIOK_BASE)
    case GPIOK_BASE:
      __HAL_RCC_GPIOK_CLK_ENABLE();
      break;
#endif
    default:
      print_error("can not find this port!");
      break;
    }
}

int led_init(struct kernel_device *dev)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    struct led *pled = ((struct led *)(dev->private_data));

    while(pled->portbase!=NULL)
    {
        led_set_port_clk(pled->portbase);
        GPIO_InitStruct.Pin = pled->pin;
        GPIO_InitStruct.Mode = pled->mode;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        
        HAL_GPIO_Init((GPIO_TypeDef *)pled->portbase, &GPIO_InitStruct);
        pled ++;
    }
    
    return 0;
}

int led_cntl(struct kernel_device *dev, int cmd, void *arg)
{
    struct led *pled = ((struct led *)(dev->private_data))+cmd;
    
    if((int)arg==0)
    {
        HAL_GPIO_WritePin((GPIO_TypeDef *)pled->portbase, pled->pin, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin((GPIO_TypeDef *)pled->portbase, pled->pin, GPIO_PIN_SET);
    }
    
    return 0;    
}

const struct device_operations ledops = {
    .init = led_init,
    .cntl = led_cntl, 
};


struct kernel_device led = {
    .ops = &ledops,
    .private_data = (void *)leds,
};

void led_hw_init(void)
{
    if(device_register("led", &led)!=0)
    {
        print_error("led kernel_device register failed!");
    }
}