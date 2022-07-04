#ifndef __I2C_EE_H
#define	__I2C_EE_H


#include "stm32f10x.h"


/**************************I2C参数定义：I2C1或I2C2********************************/
#define             TMP275_I2Cx                                I2C1
#define             TMP275_I2C_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             TMP275_I2C_CLK                             RCC_APB1Periph_I2C1
#define             TMP275_I2C_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             TMP275_I2C_GPIO_CLK                        RCC_APB2Periph_GPIOB     
#define             TMP275_I2C_SCL_PORT                        GPIOB   
#define             TMP275_I2C_SCL_PIN                         GPIO_Pin_6
#define             TMP275_I2C_SDA_PORT                        GPIOB 
#define             TMP275_I2C_SDA_PIN                         GPIO_Pin_7

/* STM32 I2C快速模式 */
#define I2C_Speed              400000  //*

/* stm32自己的地址，只要和从机地址不一样就可以 */
#define I2Cx_OWN_ADDRESS7      0X0A   



/*************************这里定义tmp275的一系列数据帧和地址****************************/
#define SLAVE_ADDRESS 1001000 //定义从机写地址（写入tmp275）
#define POINT_REGISTER_TEMPETATURE 00000000 //定义point register的地址，指向温度寄存器
#define POINT_REGISTER_CONFIGURATION 00000001 //定义point register的地址，指向初始化寄存器
#define POINT_REGISTER_TLOW 00000010//定义point register的地址，指向TLOW寄存器
#define POINT_REGISTER_THIGH 00000011//定义point register的地址，指向THIGH寄存器



/*等待超时*/
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))


/*错误信息输出*/
#define TMP275_DEBUG_ON         0

#define TMP275_INFO(fmt,arg...)           printf("<<-TMP275-INFO->> "fmt"\n",##arg)
#define TMP275_ERROR(fmt,arg...)          printf("<<-TMP275-ERROR->> "fmt"\n",##arg)
#define TMP275_DEBUG(fmt,arg...)          do{\
                                          if(TMP275_DEBUG_ON)\
                                          printf("<<-TMP275-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)



void I2C_TMP275_Init(void);   //I2C的初始化函数
uint32_t I2C_TM275_ByteWrite(u8* pBuffer, u8 WriteAddr);  //写一个字节到TMP275中
uint32_t I2C_TMP275_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead);//读
float temperature_turn(u16*temperature_buf,int bitnum); //温度数据转换

#endif /* __I2C_EE_H */
