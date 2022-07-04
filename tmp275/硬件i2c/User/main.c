/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   I2C EEPROM(AT24C02)测试，测试信息通过USART1打印在电脑的超级终端
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./i2c/IIC_for_tmp275.h"
#include <string.h>

#define  EEP_Firstpage      0x00
uint8_t I2c_Buf[2];
uint8_t I2c_Buf_Read[256];
uint8_t I2C_Test(void);


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{ 
	int i=1000;
	float temp=0;
  LED_GPIO_Config();
  
  LED_BLUE;
  /* 串口初始化 */
	USART_Config();
	

	/* I2C 外设初(AT24C02)始化 */
	I2C_TMP275_Init();
	temp=0;
	/*for(i=0;i<=7;i++)
  printf("\r\n %d \r\n",I2c_Buf_Read[i]);*/

 // printf("\r\n [0]= %d  [1]= %d\r\n",I2c_Buf_Read[0],I2c_Buf_Read[1]);
	//temp=temperature_turn(I2c_Buf_Read,8);

	if( I2C_TMP275_BufferRead(I2c_Buf_Read,POINT_REGISTER_TEMPERATURE ,2)==1)
	   temp=((short int)(I2c_Buf_Read[0]<<4)+(short int)(I2c_Buf_Read[1]>>4))*0.0625;
	//temp=(I2c_Buf_Read[0])*0.5;
	   printf("%f\r\n",temp);
		 //printf("\r\n [0]= %d  [1]= %d\r\n",I2c_Buf_Read[0],I2c_Buf_Read[1]);
		 temp=0;	 

  
  while (1)
  {    
		 
}
}


/*********************************************END OF FILE**********************/
