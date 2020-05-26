#include "1602.h"

/* �ȴ�Һ��׼���� */
void LcdWaitReady()
{
    unsigned char sta;
    
    LCD1602_DB = 0xFF;
    LCD1602_RS = 0;
    LCD1602_RW = 1;
    do {
        LCD1602_E = 1;
        sta = LCD1602_DB; //��ȡ״̬��
        LCD1602_E = 0;
    } while (sta & 0x80); //bit7����1��ʾҺ����æ���ظ����ֱ�������0Ϊֹ
}
/* ��LCD1602Һ��д��һ�ֽ����cmd-��д������ֵ */
void LcdWriteCmd(unsigned char cmd)
{
    LcdWaitReady();
    LCD1602_RS = 0;
    LCD1602_RW = 0;
    LCD1602_DB = cmd;
    LCD1602_E  = 1;
    LCD1602_E  = 0;
}
/* ��LCD1602Һ��д��һ�ֽ����ݣ�dat-��д������ֵ */
void LcdWriteDat(unsigned char dat)
{
    LcdWaitReady();
    LCD1602_RS = 1;
    LCD1602_RW = 0;
    LCD1602_DB = dat;
    LCD1602_E  = 1;
    LCD1602_E  = 0;
}
/* ������ʾRAM��ʼ��ַ���༴���λ�ã�(x,y)-��Ӧ��Ļ�ϵ��ַ����� */
void LcdSetCursor(unsigned char x, unsigned char y)
{
    unsigned char addr;
    
    if (y == 0)  //���������Ļ���������ʾRAM�ĵ�ַ
        addr = 0x00 + x;  //��һ���ַ���ַ��0x00��ʼ
    else
        addr = 0x40 + x;  //�ڶ����ַ���ַ��0x40��ʼ
    LcdWriteCmd(addr | 0x80);  //����RAM��ַ
}
/* ��Һ������ʾ�ַ�����(x,y)-��Ӧ��Ļ�ϵ���ʼ���꣬str-�ַ���ָ�� */
void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str)
{
    LcdSetCursor(x, y);   //������ʼ��ַ
    while (*str != '\0')  //����д���ַ������ݣ�ֱ����⵽������
    {
        LcdWriteDat(*str++);
    }
}
/* �򿪹�����˸Ч�� */
void LcdOpenCursor()
{
	LcdWriteCmd(0x0F);
}
/* �رչ����ʾ */
void LcdCloseCursor()
{
	LcdWriteCmd(0x0C);
}
/* ��ʼ��1602Һ�� */
void InitLcd1602()
{
    LcdWriteCmd(0x38);  //16*2��ʾ��5*7����8λ���ݽӿ�
    LcdWriteCmd(0x0C);  //��ʾ���������ر�
    LcdWriteCmd(0x06);  //���ֲ�������ַ�Զ�+1
    LcdWriteCmd(0x01);  //����
}

//*************************��ʾһ���ֽ�����
void DisplayOneChar (unsigned char X,unsigned char Y,unsigned char DData)
{
 if(Y) X|=0X40;   //Y=1��ʾ�ڶ��У�Y=0��ʾ��һ��
 X|=0X80;
 LcdWriteCmd(X); //X����ѡ����һλ
 LcdWriteCmd(DData); //DData����д����
}

//��ʾһ���ֽ��ַ�
void DisplayOneStr (unsigned char X,unsigned char Y,unsigned char DData)
{
	DisplayOneChar (X++,Y,DData/16 + '0');
	DisplayOneChar (X,Y,DData%16 + '0');
}
