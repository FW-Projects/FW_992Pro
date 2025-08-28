#include <SN8F5708.H>
#include "getkey.h"
#define uint8_t unsigned char
#define uint16_t unsigned int
#define key_init 1			   // ������ʼ������
#define key_wobble 2		   // �Ӷ�����
#define key_press 3			   // �м����´���
#define keylong_stat 4		   // ��������ʼ����
#define key_long_on 5		   // ��������������
#define key_period 6		   // �����ͷŴ���
#define keylong_pepione 50	   // ������ʱ�䳣������
#define key_continue_period 10 // �����������ڳ�������
#define keydown_ent 1		   // ����
#define keyup_ent 2
#define key_long 3
#define key_continue 4 // ������
#define key_null 0
#define key_set 1
#define key_air_up 2
#define key_air_down 3
#define key_sol_up 4
#define key_sol_down 5

sbit key_01 = P2 ^ 6;
sbit key_02 = P2 ^ 1;
sbit key_03 = P2 ^ 0;
sbit key_04 = P2 ^ 2;
sbit key_05 = P2 ^ 5;
sbit key_06 = P5 ^ 5;
volatile uint8_t key_stat = 1;
volatile last_key_temp = 0x00;
volatile uint8_t inkey_number = 0; // ���水������ֵ
volatile uint16_t key_time = 0;

key_scan(void)
{
	#if 1
	if(key_02 == 0 && key_03 == 0)
		return key_set;
	#endif
	
	#if 0
	if (key_01 == 0)
		return key_set;
	#endif
	
	if (key_02 == 0)
		return (key_air_up);
	if (key_03 == 0)
		return (key_air_down);
	if (key_04 == 0)
		return (key_sol_up);
	if (key_05 == 0)
		return (key_sol_down);
	return (key_null);
}
get_key(void) // ɨ�����
{
	uint8_t key_temp = 0;	   //
	key_temp = key_scan(); // ��ȡ�ĸ�������
	switch (key_stat)
	{
	case key_init:
	{
		if (key_null != key_temp)
			key_stat = key_wobble;
	}
	break;
	case key_wobble:
	{
		key_stat = key_press;
	}
	break;
	case key_press:
	{
		if (key_null != key_temp)
		{
			inkey_number = key_temp; // ���潡ֵ
			last_key_temp = key_temp;
			key_temp = keydown_ent;	 // ȷ�ϰ�����
			key_stat = keylong_stat; // ��λ���������
		}
		else
			key_temp = key_period; // ����ȷ�ϰ���
	}
	break;
	case keylong_stat:
	{
		if (key_null != key_temp)
		{
			if (key_time++ > keylong_pepione) // ��ⳤ����
			{
				key_stat = key_long_on; // ��λ��
				key_temp = key_long;	// ������־
				key_time = 0x00;
			}
			else
			{
				if (key_temp != last_key_temp)
					key_time = 0x00, key_stat = key_press;
				key_temp = keydown_ent; // ����ȷ�ϰ���
			}
		}
		else
			key_stat = key_period; // ����λ�������ͷ�
	}
	break;
	case key_long_on: // ��������
	{
		if (key_null != key_temp)
		{
			if (key_time++ > key_continue_period)
			{
				key_time = 0;
				key_temp = key_continue;
				key_stat = key_long_on;
			}
			else
				key_temp = key_long;
		}
		else
			key_stat = key_period;
	}
	break;
	case key_period: //                ȷ�ϰ����ͷ�
	{
		key_time = 0;
		key_stat = key_init;
		key_temp = keyup_ent;
	}
	break;
	default:
		inkey_number = key_null;
		break; // �޼�����
	}
	return key_temp;
}
