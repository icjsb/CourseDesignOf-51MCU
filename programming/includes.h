#ifndef _INCLUDES_H
#define _INCLUDES_H

#include <REG52.H>
#include "dht11.h"
#include "1602.h"
#include "configuration.h"
#include "delay.h"

#define MAIN_Fosc		11059200	//宏定义主时钟HZ

#define DS1302_W_ADDR 0x80  //写时钟日历寄存器起始地址
#define DS1302_R_ADDR 0x81	//读时钟日历寄存器起始地址

//时钟日历暂存数组，秒、分、时、日、月、周、年 初值为= 16年2月15日 周1 23:58:50

//int wdg=40,wdd=20,sdg=80,sdd=60;

#endif

