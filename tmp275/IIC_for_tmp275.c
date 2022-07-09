
#include "IIC_for_tmp275.h"
#include "./usart/bsp_usart.h"		

static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;   



static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);

/*注意：这里仅仅写了关于IIC的初始化函数，实际的操作函数*/

/**
  * @brief  I2C的IO口设置
  * @param  无
  * @retval  无
  */
static void I2C_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

	/* 使能与I2C有关的时钟 */
	TMP275_I2C_APBxClock_FUN ( TMP275_I2C_CLK, ENABLE );
	TMP275_I2C_GPIO_APBxClock_FUN ( TMP275_I2C_GPIO_CLK, ENABLE );
	
    
  /* I2C_SCL、I2C_SDA*/
  GPIO_InitStructure.GPIO_Pin = TMP275_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // 开漏输出
  GPIO_Init(TMP275_I2C_SCL_PORT, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = TMP275_I2C_SDA_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // 开漏输出
  GPIO_Init(TMP275_I2C_SDA_PORT, &GPIO_InitStructure);	
	
}


/**
  * @brief  I2C 工作模式配置
  * @param  无
  * @retval 无
  */
static void I2C_Mode_Configu(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 

  /* I2C 配置*/
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	
	/* 高低电平时间之比 */
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	/*stm32自己的地址*/
  I2C_InitStructure.I2C_OwnAddress1 =I2Cx_OWN_ADDRESS7; 
  /*使能应答信号（必须使能）*/
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	 
	/* 配置为7bit寻址模式 */
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	/* 通讯速率 */
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
  
	/* I2C初始化*/
  I2C_Init(TMP275_I2Cx, &I2C_InitStructure);
  
	/* 使能I2C */
  I2C_Cmd(TMP275_I2Cx, ENABLE);   
}


/*main函数中只用调用这个函数就可以完成I2C的初始化了*/
void I2C_TMP275_Init(void)
{

  I2C_GPIO_Config(); 
 
  I2C_Mode_Configu();

//这里存疑，到底要不要

}



/**
  * @brief   向tmp275芯片中写一个字节
  * @param   
  *		@arg pBuffer: 缓冲区地址（要往芯片中写的东西）
  *     @arg PointAddr:写入point register中的地址，选择要调用的寄存器
         参数可为：POINT_REGISTER_TEMPETATURE，POINT_REGISTER_CONFIGURATION，
		 POINT_REGISTER_TLOW，POINT_REGISTER_THIGH
  * @retval  ÎÞ
  */
uint32_t I2C_tmp275_ByteWrite(u8* pBuffer, u8 PointAddr) 
{
   
  /* Send STRAT condition */
  I2C_GenerateSTART(TMP275_I2Cx, ENABLE);

  I2CTimeout = I2CT_FLAG_TIMEOUT;  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(TMP275_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))  
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(0);
  } 
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(TMP275_I2Cx,(uint8_t)SLAVE_ADDRESS, I2C_Direction_Transmitter);
    
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(TMP275_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(1);
  }  
  
  /* 将数据写入point register中,让tmp275自己判断接下来的地址写给谁*/
  I2C_SendData(TMP275_I2Cx, PointAddr);
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(TMP275_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
  } 
  
  /* Send the byte to be written */
  I2C_SendData(TMP275_I2Cx, *pBuffer); 
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;  
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(TMP275_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
  } 
  
  /* Send STOP condition */
  I2C_GenerateSTOP(TMP275_I2Cx, ENABLE); 
  return 1;
}



/**
  * @brief   从TMP275中读数据（懒得改了，这个就是例程的函数）
  * @param   
  *		@arg pBuffer:存放读取的数据
  *		@arg WriteAddr:从这个地址读数据
  *     @arg NumByteToWrite:这个我们就设为1好了，实在懒得改了；如果是读12bit的温度，可以设为2
  * @retval  ÎÞ
  */
uint32_t I2C_TMP275_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)
{  
  
  I2CTimeout = I2CT_LONG_TIMEOUT;
  
  //*((u8 *)0x4001080c) |=0x80; 
  while(I2C_GetFlagStatus(TMP275_I2Cx, I2C_FLAG_BUSY))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(9);
   }
  
  /* Send START condition */
  I2C_GenerateSTART(TMP275_I2Cx, ENABLE);
  //*((u8 *)0x4001080c) &=~0x80;
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(TMP275_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(10);
   }
  
  /* Send TMP275 address for write */
  I2C_Send7bitAddress(TMP275_I2Cx, TMP275_ADDRESS, I2C_Direction_Transmitter);

  I2CTimeout = I2CT_FLAG_TIMEOUT;
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(TMP275_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(11);
   }
    
  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(TMP275_I2Cx, ENABLE);

  /* Send the TMP275's internal address to write to */
 /* I2C_SendData(TMP275_I2Cx, ReadAddr);  

   //IIC和EEPROM的读取应该就少这一块
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  /* Test on EV8 and clear it */
 /* while(!I2C_CheckEvent(TMP275_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(12);
   }
 */   
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(TMP275_I2Cx, ENABLE);
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(TMP275_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(13);
   }
    
  /* Send TMP275 address for read */
  I2C_Send7bitAddress(TMP275_I2Cx, TMP275_ADDRESS, I2C_Direction_Receiver);
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(TMP275_I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(14);
   }
  
  /* While there is data to be read */
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(TMP275_I2Cx, DISABLE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(TMP275_I2Cx, ENABLE);
    }

    /* Test on EV7 and clear it */    
    I2CTimeout = I2CT_LONG_TIMEOUT;
    
		while(I2C_CheckEvent(TMP275_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)==0)  
		{
			if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
		} 
    {      
      /* Read a byte from the TMP275 */
      *pBuffer = I2C_ReceiveData(TMP275_I2Cx);

      /* Point to the next location where the byte read will be saved */
      pBuffer++; 
      
      /* Decrement the read bytes counter */
      NumByteToRead--;        
    }   
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(TMP275_I2Cx, ENABLE);
  
    return 1;
}


/**
  *@brief 将温度由二进制转化为10进制
  *@arg temperature_buf:用于存放温度数据
  *@arg bitnum：温度的位数
   @retval 温度值
  */
float temperature_turn(char*temperature_buf,int bitnum); //温度数据转换
{
	int i;
	int sum=0;
	float resolution;
	int flag=1; //检查第一位判断正负
	switch(bitnum)
	{
		case 9:resolution=0.5;break;
		case 10:resolution=0.25;break;
		case 11:resolution=0.125;break;
		case 12:resolution=0.0625;break;
		default: break;
	}
	
	//判断符号
	if(temperature_buf[0]==1) flag=1;
    else flag=-1;
    
	//二进制转十进制
	for(i=1;i<=bitnum-1;i++)
	{
		sum=sum+pow(2,bitnum-i);
	}
	
	return flag*resolution*sum;
}
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  TMP275_ERROR("I2C µÈ´ý³¬Ê±!errorCode = %d",errorCode);
  
  return 0;
}


/*************************END OF FILE*************************/