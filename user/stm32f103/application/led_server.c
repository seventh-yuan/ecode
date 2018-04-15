#include "led_server.h"


static struct kernel_device *leddev = NULL;

#define led_on(dev, led) device_control(dev, led, (void *)1)
#define led_off(dev, led)   device_control(dev, led, (void *)0)


#if CONFIG_USING_RTOS==1
static void led_thread(void const *args)
{
    
    leddev = device_open("led", 0);
    led_off(leddev, 0);
    while(1)
    {
        led_on(leddev, 0);
        kernel_sleep_ms(500);
        led_off(leddev, 0);
        kernel_sleep_ms(500);
    }
}

void led_server_init(void)
{
    osThreadDef(led, led_thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    osThreadCreate(osThread(led), NULL);
    
}
#else
void led_server_init(void)
{

}
#endif
