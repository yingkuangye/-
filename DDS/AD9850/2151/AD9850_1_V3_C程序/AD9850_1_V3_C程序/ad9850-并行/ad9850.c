//***************************************************//
//函数1:           ad9850_reset()                    //
//函数2:           ad9850_reset_serial()             //
//函数3:           ad9850_wr_parrel(unsigned char w0,double frequence)//
//函数4:           ad9850_wr_serial(unsigned char w0,double frequence)//
//版本:              V1.1                            //
//日期：             2008/12/19                      //
//修改日期：         2008/12/19                      //
//编写者：         my_dds(my_dds@163.com)            //
//***************************************************//
//                  子程序说明                       //
//***************************************************//
//函数1:  ad9850_reset()
//        复位ad9850，之后为并口写入模式        
//函数2:  ad9850_reset_serial()
//        复位ad9850，之后为串口写入模式
//函数3:  ad9850_wr_parrel(unsigned char w0,double frequence)
//        并口写ad9850数据，w0为ad9850中w0的数据，frequence
//        为写入的频率	
//函数4:  ad9850_wr_serial(unsigned char w0,double frequence)
//        串口写ad9850数据，w0为ad9850中w0的数据，frequence
//        为写入的频率	
//需定义的位：
         //ad9850_w_clk    ;
         //ad9850_fq_up    ;
         //ad9850_rest     ;
         //ad9850_bit_data ;
//例：
         //sbit ad9850_w_clk    =P2^2;
         //sbit ad9850_fq_up    =P2^1;
         //sbit ad9850_rest     =P2^0;
         //sbit ad9850_bit_data =P1^7;
//***************************************************//
//                 写数据说明                        //
//***************************************************//
//写数据例：
//       ad9850_reset()
//       wr_lcd02_data(unsigned char x)
//       ad9850_wr_parrel(0x01,1000)
//       ad9850_wr_serial(0x01,1000)
//***************************************************//
//---------------------------------------------------//
//                   程序                            //
//---------------------------------------------------//
# include <reg51.h>
# include <stdio.h>
# include <intrins.h>
sbit ad9850_w_clk    =P2^2;  //P2.2口接ad9850的w_clk脚/PIN7
sbit ad9850_fq_up    =P2^1;  //P2.1口接ad9850的fq_up脚/PIN8
sbit ad9850_rest     =P2^0;  //P2.0口接ad9850的rest脚/PIN12
sbit ad9850_bit_data =P1^7;  //P1.7口接ad9850的D7脚/PIN25
//P1为8位数据口
//***************************************************//
//              ad9850复位(并口模式)                 //
//---------------------------------------------------//
void ad9850_reset()
{
ad9850_w_clk=0;
ad9850_fq_up=0;
//rest信号
ad9850_rest=0;
ad9850_rest=1;
ad9850_rest=0;
}
//***************************************************//
//              ad9850复位(并口模式)                 //
//---------------------------------------------------//
void ad9850_reset_serial()
{
ad9850_w_clk=0;
ad9850_fq_up=0;
//rest信号
ad9850_rest=0;
ad9850_rest=1;
ad9850_rest=0;
//w_clk信号
ad9850_w_clk=0;
ad9850_w_clk=1;
ad9850_w_clk=0;
//fq_up信号
ad9850_fq_up=0;
ad9850_fq_up=1;
ad9850_fq_up=0;
}
//***************************************************//
//          向ad9850中写命令与数据(并口)             //
//---------------------------------------------------//
void ad9850_wr_parrel(unsigned char w0,double frequence)
{
unsigned char w;
long int y;
double x;
//计算频率的HEX值
x=4294967295/125;//适合125M晶振
//如果时钟频率不为125MHZ，修改该处的频率值，单位MHz ！！！
frequence=frequence/1000000;
frequence=frequence*x;
y=frequence;
//写w0数据
w=w0;   
P1=w;      //w0
ad9850_w_clk=1;
ad9850_w_clk=0;
//写w1数据
w=(y>>24);
P1=w;      //w1
ad9850_w_clk=1;
ad9850_w_clk=0;
//写w2数据
w=(y>>16);
P1=w;      //w2
ad9850_w_clk=1;
ad9850_w_clk=0;
//写w3数据
w=(y>>8);
P1=w;      //w3
ad9850_w_clk=1;
ad9850_w_clk=0;
//写w4数据
w=(y>>=0);
P1=w;      //w4
ad9850_w_clk=1;
ad9850_w_clk=0;
//移入始能
ad9850_fq_up=1;
ad9850_fq_up=0;
}
//***************************************************//
//          向ad9850中写命令与数据(串口)             //
//---------------------------------------------------//
void ad9850_wr_serial(unsigned char w0,double frequence)
{
unsigned char i,w;
long int y;
double x;
//计算频率的HEX值
x=4294967295/125;//适合125M晶振
//如果时钟频率不为180MHZ，修改该处的频率值，单位MHz  ！！！
frequence=frequence/1000000;
frequence=frequence*x;
y=frequence;
//写w4数据
w=(y>>=0);
for(i=0;i<8;i++)
{
ad9850_bit_data=(w>>i)&0x01;
ad9850_w_clk=1;
ad9850_w_clk=0;
}
//写w3数据
w=(y>>8);
for(i=0;i<8;i++)
{
ad9850_bit_data=(w>>i)&0x01;
ad9850_w_clk=1;
ad9850_w_clk=0;
}
//写w2数据
w=(y>>16);
for(i=0;i<8;i++)
{
ad9850_bit_data=(w>>i)&0x01;
ad9850_w_clk=1;
ad9850_w_clk=0;
}
//写w1数据
w=(y>>24);
for(i=0;i<8;i++)
{
ad9850_bit_data=(w>>i)&0x01;
ad9850_w_clk=1;
ad9850_w_clk=0;
}
//写w0数据
w=w0;   
for(i=0;i<8;i++)
{
ad9850_bit_data=(w>>i)&0x01;
ad9850_w_clk=1;
ad9850_w_clk=0;
}
//移入始能
ad9850_fq_up=1;
ad9850_fq_up=0;
}
//***************************************************//
//                   测试程序1000Hz                  //
//---------------------------------------------------//
main()
{
P0=0x00;
P1=0x00;
P2=0x00;
P3=0x00;
//---------------------------------------------------//
//并行写1000Hz程序
ad9850_reset();
ad9850_wr_parrel(0x00,1000);
//---------------------------------------------------//
while(1)
{
}
}




