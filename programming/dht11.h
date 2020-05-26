#ifndef _DHT11_H
#define _DHT11_H

#include "includes.h"

sbit DHT_DATA = P3^7;

bit DHT_Start();

bit DHT_ByteRead(unsigned char *dat);

#endif