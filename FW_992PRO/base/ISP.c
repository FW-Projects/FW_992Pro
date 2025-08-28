#include "ISP.h"
#include <intrins.h> // for _nop_
#include <absacc.h>	 // for CBYTE
#define L_ISP_ADDR 0x3fc0
idata uint16_t r_read_rom_addr = L_ISP_ADDR;
idata uint8_t u8_data[32] = {0}; // ��дFlash������
// void ISPinit(void)
//{
//	uint8_t i = 0;
//	for(i=0;i<32;i++)
//	{
//		u8_data[i] = i;
//	}
// }

//====================================================================================================//
// дISP
//====================================================================================================//
void ISPpagewrite(uint8_t *r_ram_address) // д
{
	WDTR = 0x5a;
	PERAM = (uint8_t)r_ram_address; // ���ݻ�����(IRAM)�ĵ�һ����ַF
	PEROMH = L_ISP_ADDR >> 8;		// ҳ���(IROM)�ĵ�һ����ַ(15bit-8bit)
	PEROML = (L_ISP_ADDR & 0x00f0); // ҳ���(IROM)�ĵ�һ����ַ(7bit-5bit)
	EAL = 0;						//???                               ȫ���ж�	//�幷
	PEROML |= 0x0a;					// ��ʼISP����
	PECMD = 0x5a;					// ��ʼISP����
	EAL = 1;						//???                                ȫ���ж�
}

void ISPpageread2(void) // ��
{
	uint8_t i = 0;
	for (i = 0; i < 32; i++)
	{
		u8_data[i] = CBYTE[r_read_rom_addr++];
	}
}
