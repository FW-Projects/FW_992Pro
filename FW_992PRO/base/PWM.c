#include <SN8F5708.H>
#include "PWM.h"
#include "definition.h"

void PWM1_Init(void) 
{
	PW1M |= 0x80;
	PW1YH = 0x13; // 5000
	PW1YL = 0x88; // set PW1 16位周期控制位  周期为40mS /5000份
	PW1DH = 0x00;
	PW1DL = 0x00; // set PWM 16位占空比控制位
	PW1A = 0x00;  // A point dead band control
	PW1BH = PW1DH;
	PW1BL = PW1DL; // B point dead band control/死区控制位
}

void PWM2_Init(void)
{
	PW2M |= 0x80;
	PW2YH = 0x13; // 5000
	PW2YL = 0x88; // set PW1 16位周期控制位  周期为40mS /5000份
	PW2DH = 0x00;
	PW2DL = 0x00; // set PWM 16位占空比控制位
	PW2A = 0x00;  // A point dead band control
	PW2BH = PW2DH;
	PW2BL = PW2DL; // B point dead band control/死区控制位
}

void PWM3_init(void) /// ，
{
	PW3M |= 0x20;	   //  0xe0=Fhosc/1 0xc0=Fhosc/2 0xa0=Fhosc/4 0x80=Fhosc/8 0x60=Fhosc/16 0x40=Fhosc/32  x20=Fhosc/64 0x000=Fhosc/128频率选择,
	PW3M &= ~(1 << 3); // WM11 outputs is an inverse signal of pwm正常和反相输出控制位
	PW3M |= 1 << 1;	   // enable PWM10 output (PWCH10) 引脚选择PWM31，PWM30 ?

	PW3YH = 0x00;
	PW3YL = 0xff; // set PW1 16位周期控制位  周期为256份

	PW3DH = 0x00;
	PW3DL = 0x80; // set PWM 16位占空比控制位

	// the dead band control : B point > A point (Must)
	PW3BH = PW3DH;
	PW3BL = PW3DL; // B point dead band control/死区控制位

	PW3A = 0x00;  // A point dead band control
	P1OC |= 0x80; // 使能PWM3
}

void PWM1_out(uint16_t PWM1_d)
{
	PW1DL = PWM1_d; // B point dead band control
	PW1BL = PW1DL;
	PW1DH = PWM1_d >> 8;
	PW1BH = PW1DH;
}

void PWM2_out(uint16_t PWM2_d)
{
	PW2DL = PWM2_d; // B point dead band control
	PW2BL = PWM2_d;
	PW2DH = PWM2_d >> 8;
	PW2BH = PW2DH;
}

void PWM3_out(uint16_t PWM3_d)
{  
	PW3DL = PWM3_d;         // B point dead band control
	PW3BL = PWM3_d;
	PW3DH = PWM3_d>>8;
	PW3BH = PW3DH;
}

void PWM1_Off(void) // 关闭PWM1通道
{
	IEN4 &= 0x77;
	P1OC &= ~(1 << 5); // 关闭PWM1
	PW1M &= ~(3 << 1); // 关闭 两通道PWM
	P46 = 0;
}

void PWM2_Off(void) // 关闭PWM2通道
{
	IEN2 &= ~(1 << 1);	 // 禁止PWM2中断控制位
	IRCON2 &= ~(1 << 0); // 清中断
	P1OC &= ~(1 << 6);	 // 关闭PWM2
	PW2M &= ~(3 << 1);	 // 关闭 两通道PWM
	P50 = 0;
}

void PWM_ALL_OFF(void)
{
	PWM1_Off();
	PWM2_Off();
}

void PWM1_Open(void)
{
	PW1M &= 0xf0;
	PW1M &= ~(1 << 3);//正向输出
	PW1M |= (1 << 1);
	P1OC |= 1 << 5; // 开启PWM1
	IEN4 &= ~(1 << 3);
	IEN4 |= (1 << 7);
}

void PWM2_Open(void)
{
	PW2M &= 0xf0;
	PW2M &= ~(1 << 3);   //正向输出
	PW2M |= 1 << 1;
	P1OC |= 1 << 6;		 // 开启PWM2
	IEN2 |= 1 << 1;		 // 使能PWM2中断控制位
	IRCON2 &= ~(1 << 0); // 清中断
}

void PWM_ALL_OPEN(void)
{
	PWM1_Open();
	PWM2_Open();
}

void PWM1_ISR(void) interrupt ISRPwm1 // air_hot control
{
	if (Sol_PID_Cycle_Time != 0)
		Sol_PID_Cycle_Time--;
	PWM1_out(Sol_pid_outdata);
	
	IEN4 &= ~(1 << 3); // 清中断 (PWM1F)
}

void PWM2_ISR(void) interrupt ISRPwm2 // sol_hot control
{
	if (Air_PID_Cycle_Time != 0)
		Air_PID_Cycle_Time--;
	PWM2_out(Air_pid_outdata);
	IRCON2 &= ~(1 << 0); // 清中断 (PWM2F)
}