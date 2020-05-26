#include "delay.h"

void delay_ms(unsigned char x)   //Îó²î -0.651041666667us
{
    unsigned char a,b,c;
	for(c=x;c>0;c--)
    {
		for(b=102;b>0;b--)
        for(a=3;a>0;a--);
	}
}
void delay_30us(void)   //Îó²î -0.703125us
{
    unsigned char a;
    for(a=12;a>0;a--);
}

void delay_40us(void)   //Îó²î -0.703125us
{
    unsigned char a,b;
    for(b=3;b>0;b--)
        for(a=4;a>0;a--);
}

void delay ()
{
 int x=99999;
 while(x--);
}

void Delay_Ms1(unsigned int ms)
{
     unsigned int i;
	 do{
	      i = MAIN_Fosc / 96000; 
		  while(--i);   //96T per loop
     }while(--ms);
}