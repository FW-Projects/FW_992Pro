#include <intrins.h>
#include "PID_operation.h"
#define uint8_t unsigned char
#define uint16_t unsigned int
volatile int ek_air = 0, ek_1_air = 0, ek_sum_air = 0;
volatile int ek_sol = 0, ek_1_sol = 0, ek_sum_sol = 0;
volatile int Kp_air;
volatile int Ki_air;
volatile int Kd_air;
volatile int Kp_sol;
volatile int Ki_sol;
volatile int Kd_sol;
void PID_Init_Air(void)
{
	Kp_air = 150;
	Ki_air =1;
	Kd_air = 50;
	ek_air = 0;
	ek_1_air = 0;
	ek_sum_air = 0;
}

void PID_Init_Sol(void)
{
	Kp_sol = 250;
	Ki_sol = 1;
	Kd_sol = 100;
	ek_sol = 0;
	ek_1_sol = 0;
	ek_sum_sol = 0;
}
uint16_t PID_operation_Air(uint16_t Set_data,uint16_t Cur_data)
{
	int Kp_coe = 0, Ki_coe = 0, Kd_coe = 0, U_k = 0;
	ek_1_air = ek_air;
	ek_air = (Set_data - Cur_data); //??????
	if (ek_air > 20)
	{
		ek_air = 30;
		ek_sum_air = 0x00;
	}
	else if (ek_air < -20)
	{
		ek_air = -30;
		ek_sum_air = 0x00;
	}
	else
		ek_sum_air += ek_air;
	if (ek_sum_air > 4000)
		ek_sum_air = 4000;
	else if (ek_sum_air < -100)
		ek_sum_air = -100;
	Kp_coe = Kp_air * ek_air;
	Ki_coe = (Ki_air * ek_sum_air)>>2;
	Kd_coe = (Kd_air * (ek_air - ek_1_air));
	U_k = (Kp_coe + Ki_coe + Kd_coe);
	if (U_k < 0)
		U_k = 0;
	else if (U_k > 5000)
		U_k = 5000;
	return (U_k);
}

uint16_t PID_operation_Sol(uint16_t Set_dat,uint16_t Cur_dat)
{
	int Kp_coe_sol = 0, Ki_coe_sol = 0, Kd_coe_sol = 0, U_k_sol = 0;
	ek_1_sol = ek_sol;
	ek_sol= (Set_dat - Cur_dat); //??????
	if (ek_sol > 20)
	{
		ek_sol = 30;
		ek_sum_sol = 0x00;
	}
	else if (ek_sol < -20)
	{
		ek_sol = -30;
		ek_sum_sol = 0x00;
	}
	else
		ek_sum_sol += ek_sol;
	if (ek_sum_sol > 5000)
		ek_sum_sol = 5000;
	else if (ek_sum_sol < 0)
		ek_sum_sol = 0;
	Kp_coe_sol = Kp_sol * ek_sol;
	Ki_coe_sol = Ki_sol * ek_sum_sol;
	Kd_coe_sol = (Kd_sol * (ek_sol - ek_1_sol));
	U_k_sol = (Kp_coe_sol + Ki_coe_sol + Kd_coe_sol);
	if (U_k_sol < 0)
		U_k_sol = 0;
	else if (U_k_sol > 5000)
		U_k_sol = 5000;
	return (U_k_sol);
}