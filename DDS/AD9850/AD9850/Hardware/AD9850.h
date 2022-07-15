 //========================AD9850.h=============================//
/*--------------------------------------------
            AD9850驱动程序
----------------------------------------------*/
#ifndef __AD9850_H
#define __AD9850_H

/*选择数据通信方式，只能选择一个*/

#define MODE_SEIAL   //串行模式
//#define MODE_PARALLEL  //并行模式

#define AD9850_SYSTEM_COLCK     (1000000uL)
/* AD9850 控制和数据引脚定义 */
#define AD9850_CONTROL_PORT  GPIOA
#define AD9850_FQUD   GPIO_Pin_7
#define AD9850_WCLK   GPIO_Pin_6
#define AD9850_RST    GPIO_Pin_5

#ifdef MODE_PARALLEL  //并行模式数据口0~7
#define AD9850_DATA_PORT  GPIOE
#endif

#ifdef MODE_SEIAL   //串行模式数据口   
#define AD9850_DATA   GPIO_Pin_4  
#define AD9850_DATA_Write_1     GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_DATA,Bit_SET)
#define AD9850_DATA_Write_0     GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_DATA,Bit_RESET)
#endif 

#define AD9850_WCLK_SET    GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_WCLK,Bit_SET)
#define AD9850_WCLK_CLR    GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_WCLK,Bit_RESET)
#define AD9850_FQUD_SET    GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_FQUD,Bit_SET)
#define AD9850_FQUD_CLR    GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_FQUD,Bit_RESET)
#define AD9850_RST_SET     GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_RST,Bit_SET)
#define AD9850_RST_CLR     GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_RST,Bit_RESET)
/* 串行模式操作函数 */
#ifdef MODE_SEIAL    

void AD9850_Reset_Sreial(void) ;
void AD9850_Write_Serial(unsigned char W0,double freq) ;
#endif
/* 并行模式操作函数 */
#ifdef MODE_PARALLEL
void AD9850_Reset_Parallel(void) ;
void AD9850_Write_Parallel(unsigned char W0,double freq) ;
#endif
void Init_AD9850(void) ;
#endif /* AD8950_H */
//==================================================================/

