                                                       
#include "IIC_for_tmp275.h"
#include "./usart/bsp_usart.h"		

static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;   

extern uint8_t I2c_Buf[2];

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
  //I2C_AcknowledgeConfig(I2C1, ENABLE);  
}


/*main函数中只用调用这个函数就可以完成I2C的初始化了*/
void I2C_TMP275_Init(void)
{

  I2C_GPIO_Config(); 
 
  I2C_Mode_Configu();
	
 /* I2C_GenerateSTART(TMP275_I2Cx , ENABLE);
  //功能初始化
   while(!I2C_CheckEvent(TMP275_I2Cx , I2C_EVENT_MASTER_MODE_SELECT)); 


   //发送器件地址(写)
   I2C_Send7bitAddress(I2C1, 0X90, I2C_Direction_Transmitter);
   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  
   //发送Pointer Register
   I2C_SendData(I2C1, POINT_REGISTER_CONFIGURATION);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));


 // 写Configuration Register  12位温度 连续转换
   I2C_SendData(I2C1, 0XFE); 
   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
 

   I2C_GenerateSTOP(I2C1, ENABLE);
	 */
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
  *		@arg slaveAddr:从这个地址读数据
        @arg:pointAddr:设置point register，决定读那个寄存器
  *     @arg NumByteToWrite:读取的次数
  * @retval  ÎÞ
  */
/**
  * @brief   ´ÓEEPROMÀïÃæ¶ÁÈ¡Ò»¿éÊý¾Ý 
  * @param   
  *		@arg pBuffer:´æ·Å´ÓEEPROM¶ÁÈ¡µÄÊý¾ÝµÄ»º³åÇøÖ¸Õë
  *		@arg WriteAddr:½ÓÊÕÊý¾ÝµÄEEPROMµÄµØÖ· ´ÓÕâ¸öµØÖ·¶ÁÊý¾Ý
  *     @arg NumByteToWrite:Òª´ÓEEPROM¶ÁÈ¡µÄ×Ö½ÚÊý
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
  
  /*  ¿ªÊ¼startÃüÁî */
  I2C_GenerateSTART(TMP275_I2Cx, ENABLE);
  //*((u8 *)0x4001080c) &=~0x80;
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(TMP275_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(10);
   }
  
  /* ÔÚIIC×ÜÏßÉÏÕÒµ½TMP275µÄµØÖ· */
  I2C_Send7bitAddress(TMP275_I2Cx, SLAVE_ADDRESS, I2C_Direction_Transmitter);

  I2CTimeout = I2CT_FLAG_TIMEOUT;
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(TMP275_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(11);
   }
    
  /* Í¨¹ýÖØÐÂÉèÖÃPEÎ»À´Çå³þEV6ÊÂ¼þ */
  I2C_Cmd(TMP275_I2Cx, ENABLE);

  /* Send the TMP275's internal address to write to */
  I2C_SendData(TMP275_I2Cx, ReadAddr);  

   
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(TMP275_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
     if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(12);
   }
    
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(TMP275_I2Cx, ENABLE);
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(TMP275_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(13);
   }
    
  /* Send TMP275 address for read */
  I2C_Send7bitAddress(TMP275_I2Cx, SLAVE_ADDRESS, I2C_Direction_Receiver);
  
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

static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  TMP275_ERROR("I2C !errorCode = %d\r\n",errorCode);
  
  return 0;
}


/*************************END OF FILE*************************/
