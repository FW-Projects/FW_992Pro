#include <intrins.h> // for _nop_
#include <absacc.h>
#include "delay.h"
#define uint8_t unsigned char
#define uint16_t unsigned int

void Delay_10Us(void)
{
	uint8_t i;
	for (i = 0; i < 17; i++)
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}
}
void Delay_X10Us(uint16_t Time) // 10us
{
	uint8_t a;
	for (a = 0; a < Time; a++)
	{
		Delay_10Us();
	}
}
//===========================================================
// Function name:	DelayMs
// parameters:		Time?????? ????Time ms
// returned value:	none
//===========================================================
//void Delay_Ms(uint16_t Time)
//{
//	uint16_t a;
//	uint8_t b;
//	for (a = 0; a < Time; a++)
//	{
//		for (b = 0; b < 98; b++)
//		{
//			Delay_10Us();
//		}
//	}
//}
//===========================================================
// Function name:	DelayS
// parameters:		Time?????? ????Time S
// returned value:	none
//===========================================================
//void Delay_S(uint16_t Time)
//{
//	uint16_t a, b;
//	for (a = 0; a < Time; a++)
//	{
//		for (b = 0; b < 10; b++)
//		{
//			Delay_Ms(100);
//		}
//	}
//}

//void delay_us(uint16_t xms)
//{
//	uint16_t i, j;
//	for (i = xms; i > 0; i--)
//		for (j = 10; j > 0; j--)
//			;
//}