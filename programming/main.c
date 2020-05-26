#include "includes.h"

int WD,SD;
bit flag = 0;          //1s��ʱ��־
unsigned char T0RH = 0;  //T0����ֵ�ĸ��ֽ�
unsigned char T0RL = 0;  //T0����ֵ�ĵ��ֽ�
unsigned char TimeData[7]={50, 58, 12, 26, 5, 2, 20};

unsigned char IntToString(unsigned char *str, int dat);
void ConfigTimer0(unsigned int ms);
sbit K1 = P2^3;

/*====================================
 IO�ӿڶ���
====================================*/
sbit TSCLK = P1^0; //ʱ��
sbit TIO = P1^1;   //����
sbit TRST = P1^2;  //ʹ��



/*=================================================
*�������ƣ�DS1302_W_Byte
*�������ܣ�DS1302дһ�ֽ�����
*���룺dat��Ҫд�������
=================================================*/
void DS1302_W_Byte(unsigned char dat)
{
	unsigned char i;
	for(i = 0; i < 8; i++) //ÿ��д1bit��д8��
	{
		TSCLK = 0;		   //����ʱ������
		TIO = dat & 0x01;  //��һ�ֽ����λ��ʼд
		TSCLK = 1;		   //����ʱ�����ߣ�DS1302�����ݶ���
		dat >>= 1;		   //��������һλ 
	}	
}
/*=================================================
*�������ƣ�DS1302_R_Byte
*�������ܣ�DS1302��һ�ֽ�
*�����dat����ȡ������
=================================================*/
unsigned char DS1302_R_Byte()
{
	unsigned char i, dat;
	for(i = 0; i < 8; i++)  //ÿ��д1bit��д8��
	{
		TSCLK = 0;			//����ʱ�����ߣ�DS1302�����ݷŵ�����������
		dat >>= 1; 			//��������һλ�����ݴ����λ��ʼ�� 
		if(TIO)	dat |= 0x80;//��ȡ����
		TSCLK = 1;			//����ʱ������
	}
	return dat;				//���ض�ȡ������
}
/*=================================================
*�������ƣ�DS1302_W_DAT
*�������ܣ�дDS1302����һ��д2���ֽ�
*˵������д�����д����
*���ã�DS1302_W_Byte()
*���룺cmd����Ҫд������ ��dat����ҪЩ������
=================================================*/
void DS1302_W_DAT(unsigned char cmd, unsigned char dat)
{
	TRST = 0;			 //����ʹ�ܶ�
	TSCLK = 0;			 //������������
	TRST = 1;			 //����ʹ�ܶˣ���ʼд����
	DS1302_W_Byte(cmd);	 //д����
	DS1302_W_Byte(dat);	 //д����
}
/*=================================================
*�������ƣ�DS1302_R_DAT
*�������ܣ���DS1302����
*˵������д�������ֽں������Ӧ����
*���ã�	DS1302_W_Byte();DS1302_R_Byte();
*���룺	cmd����Ҫд������
*�����	dat������������
=================================================*/
unsigned char DS1302_R_DAT(unsigned char cmd)
{
	unsigned char dat;
	TRST = 0;			 	//����ʹ�ܶ�
	TSCLK = 0;				//������������
	TRST = 1;				//����ʹ�ܶˣ���ʼд����
	DS1302_W_Byte(cmd);		//д����
	dat = DS1302_R_Byte();	//��������
	return dat;				//���ض�������
}

/*=================================================
*�������ƣ�DS1302_Clear_WP
*�������ܣ����DS1302д����
*˵������д������0x8e��д���ƼĴ�����������üĴ���д0
*���ã�DS1302_W_DAT()
=================================================*/
void DS1302_Clear_WP()
{
	DS1302_W_DAT(0x8e,0x00);  //�ѿ��ƼĴ���WPλ��0
}
/*=================================================
*�������ƣ�DS1302_Clear_WP
*�������ܣ�����DS1302д����
*˵������д������0x8e��д���ƼĴ�����������üĴ���д0x80
*���ã�DS1302_W_DAT()
=================================================*/
void DS1302_Set_WP()
{	
	DS1302_W_DAT(0x8e,0x80); //�ѿ��ƼĴ���WPλ��1
	TRST = 0;				 //����ʹ�ܶ�
	TSCLK = 0;				 //������������
} 
/*=================================================
*�������ƣ�Set_DS1302_Time
*�������ܣ�����DS1302ʱ����������
*˵������ʱ�������ݴ�����TimeData����ת��ΪBCD�벢
	   д�뵽DS1302ʱ�������Ĵ�����
*���ã�DS1302_Clear_WP();DS1302_W_DAT();DS1302_Set_WP();
*���룺addr:��Ҫд��Ĵ����ĵ�ַ ��TimeData���飺ʱ��������ʼֵ
=================================================*/
void Set_DS1302_Time(unsigned char addr)
{
	unsigned char i, j;
	DS1302_Clear_WP();		//���д����
	for(i = 0; i < 7; i++)	//д��7���ֽڵ�ʱ�ӳ�ʼֵ
	{
		j = TimeData[i]/10;	 //BCD��ת��
		TimeData[i] %= 10;	 //BCD��ת��
		TimeData[i] += j*16; //BCD��ת��
		DS1302_W_DAT(addr, TimeData[i]); //��дDS1302ʱ��������ʼ��ַ����д����
		addr += 2;	 //ʱ�������Ĵ�����ַ+2ת����һ���Ĵ���
	}
	DS1302_Set_WP(); //����д����		
}
/*=================================================
*�������ƣ�Read_DS1302_Time
*�������ܣ���ȡDS1302ʱ������
*˵����	��ȡDS1302ʱ������ �������ݴ���ʱ�������ݴ�
		����TimeData�����ݸ�ʽBCD�룩
*���ã�DS1302_Clear_WP();DS1302_R_DAT();DS1302_Set_WP();
*���룺	addr����Ҫ��ȡʱ�������Ĵ�������ʼ��ַ
=================================================*/
void Read_DS1302_Time(unsigned char addr)
{
	unsigned char i;
	DS1302_Clear_WP();    	//���Щ����
	for(i = 0; i < 7; i++)	//��DS1302��ȡ7���ֽڵ�ʱ����������
	{
		TimeData[i] = DS1302_R_DAT(addr);//��д��Ҫ��ȡ���ݵļĴ�����ʼ��ַ���ٶ������ݴ���TimeData����
		addr += 2;						 //ʱ�������Ĵ�����ַ+2ת����һ���Ĵ���
	}
	DS1302_Set_WP();   //����д����
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
	Set_DS1302_Time(DS1302_W_ADDR);	//��д��ʱ�������Ĵ�����ʼ��ַ������ʱ��������ֵ
	
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
					str[0] = (DHT[0]/10)%10 + '0';  //ʮλתΪASCII��
					str[1] = (DHT[0]%10) + '0';  //��λתΪASCII
					LcdShowStr(3, 1, str);

					str[0] = (DHT[2]/10)%10 + '0';  //ʮλתΪASCII��
					str[1] = (DHT[2]%10) + '0';  //��λתΪASCII
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
	unsigned long tmp;  //��ʱ����
    
  tmp = 11059200 / 12;      //��ʱ������Ƶ��
  tmp = (tmp * ms) / 1000;  //��������ļ���ֵ
  tmp = 65536 - tmp;        //���㶨ʱ������ֵ
  tmp = tmp + 12;           //�����ж���Ӧ��ʱ��ɵ����
  T0RH = (unsigned char)(tmp>>8);  //��ʱ������ֵ���Ϊ�ߵ��ֽ�
  T0RL = (unsigned char)tmp;
  TMOD &= 0xF0;   //����T0�Ŀ���λ
  TMOD |= 0x01;   //����T0Ϊģʽ1
  TH0 = T0RH;     //����T0����ֵ
  TL0 = T0RL;
  ET0 = 1;        //ʹ��T0�ж�
  TR0 = 1;        //����T0
}

unsigned char IntToString(unsigned char *str, int dat)
{
    signed char i = 0;
    unsigned char len = 0;
    unsigned char buf[6];
    
    if (dat < 0)  //���Ϊ����������ȡ����ֵ������ָ������Ӹ���
    {
        dat = -dat;
        *str++ = '-';
        len++;
    }
    do {          //��ת��Ϊ��λ��ǰ��ʮ��������
        buf[i++] = dat % 10;
        dat /= 10;
    } while (dat > 0);
    len += i;     //i����ֵ������Ч�ַ��ĸ���
    while (i-- > 0)   //������ֵת��ΪASCII�뷴�򿽱�������ָ����
    {
        *str++ = buf[i] + '0';
    }
    *str = '\0';  //����ַ���������
    
    return len;   //�����ַ�������
}

void InterruptTimer0() interrupt 1
{
    static unsigned char tmr1s = 0;
   
    TH0 = T0RH;  //���¼�������ֵ
    TL0 = T0RL;
    tmr1s++;
    if (tmr1s >= 10)  //��ʱ1s
    {
			flag = 1;
      tmr1s = 0;
        
	}
}
