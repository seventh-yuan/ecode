/**
  *******************************************************
  *@file        startup.c
  *@author      Arno
  *@version     V1.0.0
  *@date        2018/4/1
  *@brief       xtime  module.
  *             This is the kernel part of ecode.
  *******************************************************
  */
/* Includes----------------------------------------------*/
#include "include.h"


/*Private variables--------------------------------------*/
/*Public variables---------------------------------------*/
/*Private functions--------------------------------------*/
/*Public functions---------------------------------------*/
extern void driver_init(void);
extern void application_init(void);

/**
  * @brief This function is used to init board and application.
  * @retval None
  */
void startup(void)
{
    driver_init();
    
    application_init();
}

/**
  * @brief This function is main entry.
  * @retval 0
  */
int main(void)
{
    kernel_init();
    
    startup();
    
    kernel_start();
    
    return 0;
}