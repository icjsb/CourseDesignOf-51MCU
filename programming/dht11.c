#include "dht11.h"

bit DHT_Start()
{	
	unsigned char m=0;
	DHT_DATA=1;
	DHT_DATA=0;	
	delay_ms(20);
	DHT_DATA=1;
	delay_40us();
	if(DHT_DATA==1)
		return 0;
	else 
		while((DHT_DATA==0)&&(m<200))m++;
	delay_40us();
	delay_40us();
	return 1;				
}
bit DHT_ByteRead(unsigned char *dat)
{	
	unsigned char temp=0;
	unsigned char x,y;
	unsigned char m=0;
	unsigned char n=0;
	unsigned char mask=0x01;
	unsigned char sum=0;
	for(y=0;y<5;y++)
	{
		for(mask=0x80;mask!=0;mask>>=1)
		{	
			while(DHT_DATA==0&&m<200)m++;
			delay_30us();
			if(DHT_DATA)
				temp|=mask;
			else
				temp&=(~mask);
			while(DHT_DATA==1&&n<200)n++;
		}
		*(dat+y)=temp;
		temp=0;
	}
	for(x=0;x<4;x++)
		sum+=*(dat+x);	
	if((sum&=0xff)==*(dat+4))
		return 1;
	else 
		return 0;
}

