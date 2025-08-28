#include "lcd.h"
#include <SN8F5708.h>
#include "definition.h"

#define ON 1
#define OFF 0

uint8_t code table[] = {0x50, 0xde, 0xb0, 0x94, 0x1e, 0x15, 0x11, 0xdc, 0x10, 0x14, 0x61, 0x1a, 0x15, 0xbf, 0x71, 0xff, 0x38, 0x39, 0x58, 0xbb};
// 0    1    2    3    4    5    6    7    8    9    E    H    S    -    C   È«Ãð  P    F    N    r

void led_out(uint16_t led_data)
{
	P0 = led_data;
}

void Disp_Air_Temp(uint16_t in_data)
{
	uint8_t ddd;
	ddd = in_data / 100;
	Air_bai = table[ddd];
	ddd = in_data % 100 / 10;
	Air_shi = table[ddd];
	ddd = in_data % 10;
	Air_ge = table[ddd];
}

void Disp_Sol_Temp(uint16_t in_dat)
{
	uint8_t dd;
	dd = in_dat / 100;
	Sol_bai = table[dd];
	dd = in_dat % 100 / 10;
	Sol_shi = table[dd];
	dd = in_dat % 10;
	Sol_ge = table[dd];
}

void Disp_Air_Speed(uint8_t in_air)
{
	uint8_t aa;
	aa = in_air / 10;
	Air_speed_shi = table[aa];
	aa = in_air % 10;
	Air_speed_ge = table[aa];
}

void Disp_Air_Speed_Sleep(void)
{
	Air_speed_shi = table[13];
	Air_speed_ge = table[13];
}

/* Error Disp */
void Disp_Air_Error(uint8_t in_err)
{
	Air_bai = table[10]; // E
	Air_shi = table[19]; // r
	Air_ge = table[in_err];
}

void Disp_Sol_Error(uint8_t in_err)
{
	Sol_bai = table[10]; // E
	Sol_shi = table[19]; // r
	Sol_ge = table[in_err];
}

/* Sleep Disp */
void Disp_Air_Sleep(void)
{
	Air_bai = table[13];	   // -
	Air_shi = table[13];	   // -
	Air_ge = table[13];		   // -
	Air_speed_shi = table[13]; // -
	Air_speed_ge = table[13];  // -
}
void Disp_Sol_Sleep(void)
{
	Sol_bai = table[13]; // -
	Sol_shi = table[13]; // -
	Sol_ge = table[13];	 // -
}

void Disp_Air_Off(void)
{
	//??
	Air_bai = table[15];	
	Air_shi = table[15]; 
	Air_ge = table[15];	 
}

void Disp_Sol_Off(void)
{
	//??
	Sol_bai = table[15];	
	Sol_shi = table[15]; 
	Sol_ge = table[15];	
}
void Disp_Air_Speed_Off(void)
{
	//??
	Air_speed_shi = table[15]; 
	Air_speed_ge = table[15]; 
}


/* ON/OFF Disp */
void Disp_On_Off(uint8_t in_on_off)
{
	if (in_on_off == ON)
	{
		Sol_bai = table[15];
		Sol_shi = table[0]; // O
		Sol_ge = table[18]; // N
	}
	else
	{
		Sol_bai = table[0];	 // O
		Sol_shi = table[17]; // F
		Sol_ge = table[17];	 // F
	}
}

///* P0X Disp*/
// void Disp_P0X(uint8_t in_p)
//{
//	Air_bai = table[16];  // P
//	Air_shi = table[0];	  // 0
//	Air_ge = table[in_p]; // X
// }

void Disp_cf(uint8_t in_cf)
{
	if (in_cf == 0)
	{
		Sol_bai = table[13]; // -
		Sol_shi = table[14]; // C
		Sol_ge = table[13];	 // -
	}
	else
	{
		Sol_bai = table[13]; // -
		Sol_shi = table[17]; // F
		Sol_ge = table[13];	 // -
	}
}

void Disp_clean(void)
{
	Sol_bai = 0xff;
	Sol_shi = 0xff;
	Sol_ge = 0xff;
}

/*
 * ???????? ????/???
 * ???in_number ??? in_menu_level ????
 */

void disp_Px_flicker(uint8_t in_num)
{
	if (P_ss_flag == 1 && P_ss_bz == 1)
	{
		Air_bai = table[16];	// P
		Air_shi = table[0];		// 0
		Air_ge = table[in_num]; // X
		P_ss_bz = 0;
	}
	else if (P_ss_flag == 0 && P_ss_bz == 0)
	{
		Air_bai = 0xff; // P
		Air_shi = 0xff; // 0
		Air_ge = 0xff;	// X
		P_ss_bz = 1;
	}
}

void disp_Px_longLight(uint8_t in_num)
{
	Air_bai = table[16];	// P
	Air_shi = table[0];		// 0
	Air_ge = table[in_num]; // X
}

void Disp_First_menu_number(uint8_t in_number, inmenu_level) // ????
{
//	if (inmenu_level == 1) // ???????????
//	{
//		disp_Px_flicker(in_number); // ???????
//	}
//	else // ????????????
//	{
//		disp_Px_longLight(in_number); // ????????
//	}
	disp_Px_longLight(in_number); // ????????
}

/* AIR DISP HANDLE*/
void Disp_Air_Handle(uint8_t in_disp_num)
{
	switch (in_disp_num)
	{
	case DISP_REALTEMP:
		/* code */
		Disp_Air_Temp(Air_real_temp_data);
		break;
	case DISP_REALTEMP_Fahrenheit:
		/* code */
		Disp_Air_Temp((Air_real_temp_data * 9 / 5) + 32);
		break;
	case DISP_SETTEMP:
		/* code */
		Disp_Air_Temp(air_key_data);
		break;
	// case DISP_SETTEMP_Fahrenheit:
	// 	/* code */
	// 	Disp_Air_Temp((Air_set_temp_data * 9 / 5) + 32);
	// 	break;
	case DISP_ERR:
		/* code */
		Disp_Air_Error(Error_num_Air);
		break;
	case DISP_SLEEP:
		/* code */
		Disp_Air_Sleep();
		break;
	case DISP_OFF:
		/* code */
		Disp_Air_Off();
		break;
	default:
		break;
	}
}

/* SOL DISP HANDLE*/
void Disp_Sol_Handle(uint8_t in_disp_num)
{
	switch (in_disp_num)
	{
	case DISP_REALTEMP:
		/* code */
		Disp_Sol_Temp(Sol_real_temp_data);
		break;
	case DISP_REALTEMP_Fahrenheit:
		/* code */
		Disp_Sol_Temp((Sol_real_temp_data * 9 / 5) + 32);
		break;
	case DISP_SETTEMP:
		/* code */
		Disp_Sol_Temp(sol_key_data);
		break;
	case DISP_ERR:
		/* code */
		Disp_Sol_Error(Error_num_Sol);
		break;
	case DISP_SLEEP:
		/* code */
		Disp_Sol_Sleep();
		break;
	case DISP_OFF:
		/* code */
		Disp_Sol_Off();
		break;
	default:
		break;
	}
}

void Disp_Fan_Handle(uint8_t in_disp_num)
{
	switch (in_disp_num)
	{
	case DISP_WORK:
		/* code */
		Disp_Air_Speed(Air_speed_data);
		break;
	case DISP_SLEEP:
		/* code */
		Disp_Air_Speed_Sleep();
		break;
	case DISP_SLEEPING:
		Disp_Air_Speed(99);
		break;
	case DISP_OFF:
		/* code */
		Disp_Air_Speed_Off();
		break;
	default:
		break;
	}
}

/* SET DISP HANDLE*/

void dispmenu_firstpageoption(uint8_t in_First_menu_number) // ??????
{
	Disp_First_menu_number(in_First_menu_number, 1); // ???????????Px
	switch (in_First_menu_number)
	{
	case 0:				 // ????
		Disp_On_Off(ON); // ????ON
		break;
	case 1: // ??????
		Disp_Sol_Temp(air_set_cal_data);
		break;
	case 2: // ??????
		Disp_Sol_Temp(sol_set_cal_data);
		break;
	case 3: // ??????
		Disp_cf(set_cf_flag);
		break;
	case 4: // ????
		Disp_On_Off(disp_gate);
		break;
	case 5:
		Disp_On_Off(speak_gate);
		break;
	case 6: // ????
		Disp_Sol_Temp(set_sleep_time);
		break;
	default:
		break;
	}
}

//void dispmenu_secondpageoption(uint8_t in_Second_menu_number) // ??????????
//{
//	Disp_First_menu_number(in_Second_menu_number, 2); // ??????????Px
//	if (ss_flag == 1 && last_state_flag == 1)
//	{
//		last_state_flag = 0;
//		switch (in_Second_menu_number)
//		{
//		case 0: // ???? ??????????????????????
//			// disp_On_Off(ON);	//????ON
//			_nop_();
//			break;
//		case 1: // COM1????
//			Disp_Sol_Temp(air_set_cal_data);
//			break;
//		case 2: // COM2????
//			Disp_Sol_Temp(sol_set_cal_data);
//			break;
//		case 3: // ??????
//			Disp_cf(set_cf_flag);
//			break;
//		case 4: // ????
//			Disp_On_Off(disp_gate);
//			break;
//		case 5: // ????
//			Disp_Sol_Temp(set_sleep_time);
//			break;
//		default:
//			break;
//		}
//	}
//	else if (ss_flag == 0 && last_state_flag == 0)
//	{
//		Disp_clean();
//		last_state_flag = 1;
//	}
//}

void Disp_Set_Handle(uint8_t in_First_menu_number)
{
	Disp_First_menu_number(in_First_menu_number, 1); //???Px

	dispmenu_firstpageoption(in_First_menu_number); 
}
