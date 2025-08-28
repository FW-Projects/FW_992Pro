#include "SN8F5708.H"
#include "tmr.h"
#include "definition.h"
#include "lcd.h"
/*  T0 tmr  */
#define T0Mode0     0 << 0    //T0 mode0,13bit counter
#define T0Mode1     1 << 0    //T0 mode1,16bit counter
#define T0Mode2     2 << 0    //T0 mode2,8bit auto-reload counter
#define T0Mode3     3 << 0    //T0 mode3,T0 two 8bit counter / T1 no flag
#define T0Gate      8 << 0    //T0 gating clock by INT0
#define T0ClkFcpu   0 << 0    //T0 clock source from Fcpu / 12
#define T0ClkExt    4 << 0    //T0 clock source from Fosc or FRTC
#define T0ExtFosc   0 << 4    //T0 clock source from Fosc
#define T0ExtFRTC   8 << 4    //T0 clock source from FRTC


/*  T1 tmr  */
#define T1Mode0     0 << 4    //T1 mode0,13bit counter
#define T1Mode1     1 << 4    //T1 mode1,16bit counter
#define T1Mode2     2 << 4    //T1 mode2,8bit auto-reload counter
#define T1Mode3     3 << 4    //T1 mode3,T1 stop
#define T1Gate      8 << 4    //T1 gating clock by INT1
#define T1ClkFcpu   0 << 4    //T1 clock source from Fcpu / 12
#define T1ExtFosc   4 << 4    //T1 clock source from Fosc


sbit COM0 = P1 ^ 2; // COM0
sbit COM1 = P1 ^ 1; // COM1
sbit COM2 = P1 ^ 0; // COM2
sbit COM3 = P1 ^ 5; // COM3
sbit COM4 = P1 ^ 4; // COM4
sbit COM5 = P1 ^ 3; // COM5
sbit COM6 = P1 ^ 6;   // COM6
sbit COM7 = P1 ^ 7;   // COM7

void InitT0(void)
{
	//T0 Initial 从0x00开始计数
	TH0 = 0x00;
	TL0 = 0x00;
	
	//T0 mode1,16bit counter & Fcpu/12
	TMOD |= T0Mode1 | T0ClkFcpu;
	
	//T0 enable & Clear TF0
	TR0 = 1;
	TF0 = 0;
	
	//enable T0 interrupt
	ET0 = 1;
	//enable total interrupt
	EAL = 1;
	
}

//void InitT1(void)
//{	
//	//T1 mode1,16bit counter & Fcpu/12
//	TMOD |= T1Mode1 | T1ClkFcpu;
//	
//	//T1 enable & Clear TF0
//	TR1 = 1;
//	TF1 = 0;

//	//从初值55536开始计数
//	TH1 = (65536 - 10000) / 256;
//	TL1 = (65536 - 10000) % 256;

//	//低优先级
//	IP0 &= ~(1 << 3);
//	IP1 &= ~(1 << 3);
//	
//	//enable T1 interrupt
//	ET1 = 1;
//	//enable total interrupt
//	EAL = 1;
//}




//void StartT0(void)
//{
//	TR0 = 1;
//	ET0 = 1;
//}

//void StopT0(void)
//{
//	TR0 = 0;
//	ET0 = 0;
//}


/*  T0 interrupt service routine  */
void T0_ISR(void) interrupt 1
{
	COM0 = 1;
	COM1 = 1;
	COM2 = 1;
	COM3 = 1;
	COM4 = 1;
	COM5 = 1;
	COM6 = 1;
	COM7 = 1;
	switch (Disp_Com) // 显示扫描
	{
	case 1:
		led_out(Sol_bai);
		COM0 = 0;
		break;
	case 2:
		led_out(Sol_shi);
		COM1 = 0;
		break;
	case 3:
		led_out(Sol_ge);
		COM2 = 0;
		break;
	case 4:
		led_out(Air_bai);
		COM3 = 0;
		break;
	case 5:
		led_out(Air_shi);
		COM4 = 0;
		break;
	case 6:
		led_out(Air_ge);
		COM5 = 0;
		break;
	case 7:
		led_out(Air_speed_shi);
		COM6 = 0;
		break;
	case 8:
		led_out(Air_speed_ge);
		COM7 = 0;
		Disp_Com = 0;
		break;
	default:Disp_Com = 0;
		break;
	}
	Disp_Com++;
	if (Sol_Sleep_Ready_Flag == on)
	{
		if(Sol_Sleep_Time != 0x00)
		{
			if(Cycle_sleep_time != 0x00)
				Cycle_sleep_time--;
			else
			{
				Cycle_sleep_time = 0x7B56;
				Sol_Sleep_Time--;
			}	
		}
		else
			Sol_Sleep_Time = 0x00;
	}
//	else{
//		Sol_Sleep_Time = set_sleep_time;
//		Cycle_sleep_time = 0x7B56;
//	}
	if (Time_colck_1 == 0x00)
	{
		if(Cycle_Times_Error != 0x00)
			Cycle_Times_Error--;
		if(Cycle_Times_Disp_Air != 0x00)
			Cycle_Times_Disp_Air--;
		if(Cycle_Times_Disp_Sol != 0x00)
			Cycle_Times_Disp_Sol--;
		if(Ec_End_Times != 0x00)
			Ec_End_Times--;
		if(key_out_time != 0x00)
			key_out_time--;
		if(init_outtime != 0x00)
			init_outtime--;
		if (flicker_time != 0)
			flicker_time--;
		else
		{
			ss_flag = !ss_flag;
			P_ss_flag = !P_ss_flag;
			flicker_time = 0x0a;
		}
		
		Time_colck_1 = 26;
	}
	else
		Time_colck_1--;
	if (Key_Cycle_Times != 0)
		Key_Cycle_Times--;
	if(Ec_Cycle_Times != 0x00)
		Ec_Cycle_Times--;
	TF0 = 0; // Clear TF0
	TH0 = (65536 - 5000) / 256;
	TL0 = (65536 - 5000) % 256;
	EAL = 1;
	ET0 = 1;
	TR0 = 1;
}

/*  T1 interrupt service routine  */
void T1_ISR(void) interrupt 3
{
	TF1 = 0; // Clear TF1
	TH1 = (65536 - 10000) / 256;
	TL1 = (65536 - 10000) % 256;
}