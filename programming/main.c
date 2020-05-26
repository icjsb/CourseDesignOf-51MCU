#include "includes.h"

int WD,SD;
bit flag = 0;          //1s定时标志
unsigned char T0RH = 0;  //T0重载值的高字节
unsigned char T0RL = 0;  //T0重载值的低字节
unsigned char TimeData[7]={50, 58, 12, 26, 5, 2, 20};

unsigned char IntToString(unsigned char *str, int dat);
void ConfigTimer0(unsigned int ms);
sbit K1 = P2^3;

/*====================================
 IO接口定义
====================================*/
sbit TSCLK = P1^0; //时钟
sbit TIO = P1^1;   //数据
sbit TRST = P1^2;  //使能



/*=================================================
*函数名称：DS1302_W_Byte
*函数功能：DS1302写一字节数据
*输入：dat：要写入的数据
=================================================*/
void DS1302_W_Byte(unsigned char dat)
{
	unsigned char i;
	for(i = 0; i < 8; i++) //每次写1bit，写8次
	{
		TSCLK = 0;		   //拉低时钟总线
		TIO = dat & 0x01;  //从一字节最低位开始写
		TSCLK = 1;		   //拉高时钟总线，DS1302把数据读走
		dat >>= 1;		   //数据右移一位 
	}	
}
/*=================================================
*函数名称：DS1302_R_Byte
*函数功能：DS1302读一字节
*输出：dat：读取的数据
=================================================*/
unsigned char DS1302_R_Byte()
{
	unsigned char i, dat;
	for(i = 0; i < 8; i++)  //每次写1bit，写8次
	{
		TSCLK = 0;			//拉低时钟总线，DS1302把数据放到数据总线上
		dat >>= 1; 			//数据右移一位，数据从最低位开始读 
		if(TIO)	dat |= 0x80;//读取数据
		TSCLK = 1;			//拉高时钟总线
	}
	return dat;				//返回读取的数据
}
/*=================================================
*函数名称：DS1302_W_DAT
*函数功能：写DS1302数据一次写2个字节
*说明：先写命令后写数据
*调用：DS1302_W_Byte()
*输入：cmd：需要写的命令 ，dat：需要些的数据
=================================================*/
void DS1302_W_DAT(unsigned char cmd, unsigned char dat)
{
	TRST = 0;			 //拉低使能端
	TSCLK = 0;			 //拉低数据总线
	TRST = 1;			 //拉高使能端，开始写数据
	DS1302_W_Byte(cmd);	 //写命令
	DS1302_W_Byte(dat);	 //写数据
}
/*=================================================
*函数名称：DS1302_R_DAT
*函数功能：读DS1302数据
*说明：先写入命令字节后读出对应数据
*调用：	DS1302_W_Byte();DS1302_R_Byte();
*输入：	cmd：需要写的命令
*输出：	dat：读出的数据
=================================================*/
unsigned char DS1302_R_DAT(unsigned char cmd)
{
	unsigned char dat;
	TRST = 0;			 	//拉低使能端
	TSCLK = 0;				//拉低数据总线
	TRST = 1;				//拉高使能端，开始写数据
	DS1302_W_Byte(cmd);		//写命令
	dat = DS1302_R_Byte();	//读出数据
	return dat;				//返回读出数据
}

/*=================================================
*函数名称：DS1302_Clear_WP
*函数功能：清除DS1302写保护
*说明：先写入命令0x8e（写控制寄存器）接着向该寄存器写0
*调用：DS1302_W_DAT()
=================================================*/
void DS1302_Clear_WP()
{
	DS1302_W_DAT(0x8e,0x00);  //把控制寄存器WP位置0
}
/*=================================================
*函数名称：DS1302_Clear_WP
*函数功能：设置DS1302写保护
*说明：先写入命令0x8e（写控制寄存器）接着向该寄存器写0x80
*调用：DS1302_W_DAT()
=================================================*/
void DS1302_Set_WP()
{	
	DS1302_W_DAT(0x8e,0x80); //把控制寄存器WP位置1
	TRST = 0;				 //拉低使能端
	TSCLK = 0;				 //拉低数据总线
} 
/*=================================================
*函数名称：Set_DS1302_Time
*函数功能：设置DS1302时钟日历数据
*说明：把时钟日历暂存数组TimeData数据转换为BCD码并
	   写入到DS1302时钟日历寄存器中
*调用：DS1302_Clear_WP();DS1302_W_DAT();DS1302_Set_WP();
*输入：addr:需要写入寄存器的地址 ，TimeData数组：时钟日历初始值
=================================================*/
void Set_DS1302_Time(unsigned char addr)
{
	unsigned char i, j;
	DS1302_Clear_WP();		//清除写保护
	for(i = 0; i < 7; i++)	//写入7个字节的时钟初始值
	{
		j = TimeData[i]/10;	 //BCD码转换
		TimeData[i] %= 10;	 //BCD码转换
		TimeData[i] += j*16; //BCD码转换
		DS1302_W_DAT(addr, TimeData[i]); //先写DS1302时钟日历起始地址，再写数据
		addr += 2;	 //时钟日历寄存器地址+2转向下一个寄存器
	}
	DS1302_Set_WP(); //开起写保护		
}
/*=================================================
*函数名称：Read_DS1302_Time
*函数功能：读取DS1302时钟数据
*说明：	读取DS1302时钟数据 返回数据存入时钟日历暂存
		数组TimeData（数据格式BCD码）
*调用：DS1302_Clear_WP();DS1302_R_DAT();DS1302_Set_WP();
*输入：	addr：需要读取时钟日历寄存器的起始地址
=================================================*/
void Read_DS1302_Time(unsigned char addr)
{
	unsigned char i;
	DS1302_Clear_WP();    	//清楚些保护
	for(i = 0; i < 7; i++)	//从DS1302读取7个字节的时钟日历数据
	{
		TimeData[i] = DS1302_R_DAT(addr);//先写入要读取数据的寄存器起始地址，再读出数据存入TimeData数组
		addr += 2;						 //时钟日历寄存器地址+2转向下一个寄存器
	}
	DS1302_Set_WP();   //开起写保护
} 


void main()
{
	bit tmp;
	unsigned char str[12];
	unsigned char str1[12];
	unsigned char DHT[5];
	
	EA = 1;
	ConfigTimer0(10);
	InitLcd1602();
	DHT_Start();
	Set_DS1302_Time(DS1302_W_ADDR);	//先写入时钟日历寄存器起始地址再设置时钟日历初值
	
	delay();
	delay();
	delay();
	
	LcdShowStr(0,0,"WD");
	LcdShowStr(2,0,":");
	LcdShowStr(0,1,"SD");
	LcdShowStr(2,1,":");
	
	
	while(1)
	{		
			Read_DS1302_Time(DS1302_R_ADDR);
			DHT_Start();
			tmp = DHT_ByteRead(&DHT);
			
		  if(flag)
			{
				if(tmp == 1)
				{
					str[0] = (DHT[0]/10)%10 + '0';  //十位转为ASCII码
					str[1] = (DHT[0]%10) + '0';  //个位转为ASCII
					LcdShowStr(3, 1, str);

					str[0] = (DHT[2]/10)%10 + '0';  //十位转为ASCII码
					str[1] = (DHT[2]%10) + '0';  //个位转为ASCII
					str[2] = '\0';
					LcdShowStr(3,0, str);

					WD= DHT[2];
					SD=DHT[0];
				}
		str1[0] = TimeData[6]/16 + '0';
		str1[1] = TimeData[6]%16 + '0';
		LcdShowStr(6, 0, str1);
		LcdShowStr(8,0,"-");
		
		str1[0] = TimeData[4]/16 + '0';
		str1[1] = TimeData[4]%16 + '0';
		LcdShowStr(9, 0, str1);
    LcdShowStr(11,0,"-");
				
		str1[0] = TimeData[3]/16 + '0';
		str1[1] = TimeData[3]%16 + '0';
		LcdShowStr(12,0, str1);
		
		
		str1[0] = TimeData[2]/16 + '0';
		str1[1] = TimeData[2]%16 + '0';
		LcdShowStr(6,1, str1);
		LcdShowStr(8,1,":");
		
		str1[0] = TimeData[1]/16 + '0';
		str1[1] = TimeData[1]%16 + '0';
		LcdShowStr(9,1, str1);
		LcdShowStr(11,1,":");
		
		str1[0] = TimeData[0]/16 + '0';
		str1[1] = TimeData[0]%16 + '0';
		LcdShowStr(12,1, str1);
		
			}
				
		
	
		//Delay_Ms1(1000);	
				

  	}

	
}


void ConfigTimer0(unsigned int ms)
{
	unsigned long tmp;  //临时变量
    
  tmp = 11059200 / 12;      //定时器计数频率
  tmp = (tmp * ms) / 1000;  //计算所需的计数值
  tmp = 65536 - tmp;        //计算定时器重载值
  tmp = tmp + 12;           //补偿中断响应延时造成的误差
  T0RH = (unsigned char)(tmp>>8);  //定时器重载值拆分为高低字节
  T0RL = (unsigned char)tmp;
  TMOD &= 0xF0;   //清零T0的控制位
  TMOD |= 0x01;   //配置T0为模式1
  TH0 = T0RH;     //加载T0重载值
  TL0 = T0RL;
  ET0 = 1;        //使能T0中断
  TR0 = 1;        //启动T0
}

unsigned char IntToString(unsigned char *str, int dat)
{
    signed char i = 0;
    unsigned char len = 0;
    unsigned char buf[6];
    
    if (dat < 0)  //如果为负数，首先取绝对值，并在指针上添加负号
    {
        dat = -dat;
        *str++ = '-';
        len++;
    }
    do {          //先转换为低位在前的十进制数组
        buf[i++] = dat % 10;
        dat /= 10;
    } while (dat > 0);
    len += i;     //i最后的值就是有效字符的个数
    while (i-- > 0)   //将数组值转换为ASCII码反向拷贝到接收指针上
    {
        *str++ = buf[i] + '0';
    }
    *str = '\0';  //添加字符串结束符
    
    return len;   //返回字符串长度
}

void InterruptTimer0() interrupt 1
{
    static unsigned char tmr1s = 0;
   
    TH0 = T0RH;  //重新加载重载值
    TL0 = T0RL;
    tmr1s++;
    if (tmr1s >= 10)  //定时1s
    {
			flag = 1;
      tmr1s = 0;
        
	}
}
