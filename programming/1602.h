#ifndef _1602_H
#define _1602_H

#include "includes.h"

#define LCD1602_DB P0
sbit LCD1602_RS = P2^0;
sbit LCD1602_RW = P2^1;
sbit LCD1602_E = P2^2;

void LcdWaitReady();

void LcdWriteCmd(unsigned char cmd);

void LcdWriteDat(unsigned char dat);

void LcdCursor(unsigned char x, unsigned char y);

void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);

void LcdOpenCursor();

void LcdCloseCursor();

void InitLcd1602();

void DisplayOneChar (unsigned char X,unsigned char Y,unsigned char DData);

void DisplayOneStr (unsigned char X,unsigned char Y,unsigned char DData);


#endif