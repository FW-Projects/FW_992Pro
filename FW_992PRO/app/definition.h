#ifndef __DEFINITION_H__
#define __DEFINITION_H__
#include <SN8F5708.H>

#define uint8_t unsigned char
#define uint16_t unsigned int

#define falsh 0
#define true 1
#define on 1
#define off 0
#define CLOSE 0
#define SLEEP 1
#define RUN 2


#define DISP_REALTEMP 1
#define DISP_REALTEMP_Fahrenheit 2
#define DISP_SETTEMP 3
#define DISP_SETTEMP_Fahrenheit 4
#define DISP_ERR 5
#define DISP_SLEEP 6
#define DISP_WORK 7
#define DISP_OFF 8
#define DISP_SLEEPING 9

#define first_menu_level 1
#define second_menu_level 2

#define  pid_hot 1
#define add_hot 2
#define sub_hot 3

#define AUTO 1
#define MANUAL 0

#define OUT 0
#define AIR_COMPENSATION 1
#define SOL_COMPENSATION 2
#define CF_MODE 3
#define DISPLAY_MODE 4
#define SPEAK_MODE 5
#define SLEEP_TIME 6


sbit AIR_HOT_KEY = P5 ^ 4;         // 风枪加热开关
sbit Air_Check_Sleep = P5 ^ 3; // 风枪睡眠检测
sbit Sol_Check_Sleep = P3 ^ 3; // 烙铁睡眠检测
sbit Air_Check_OffOn = P4 ^ 7; // 风枪开关检测
sbit Sol_Check_OffOn = P5 ^ 1; // 烙铁开关检测
sbit Air_Sleep_Mode = P5 ^ 5;  // 风枪睡眠模式

/* display definition */
extern uint8_t Disp_Number_Air;
extern uint8_t Disp_Number_Sol;
extern uint8_t Disp_Number_Air_Speed;
extern uint8_t Cycle_Times_Disp_Air;
extern uint8_t Cycle_Times_Disp_Sol;
extern uint8_t Air_bai;
extern uint8_t Air_shi;
extern uint8_t Air_ge;
extern uint8_t Sol_bai, Sol_shi, Sol_ge;
extern uint8_t Air_speed_shi, Air_speed_ge;

/* PID definition */
extern uint8_t Air_PID_Cycle_Time;
extern uint8_t Sol_PID_Cycle_Time;
extern uint16_t Air_pid_outdata;
extern uint16_t Sol_pid_outdata;
extern uint8_t pid_mode_air;
extern uint8_t pid_mode_sol;

/* data definition*/
extern uint16_t error_air_data;
extern uint16_t error_sol_data;
extern uint16_t Air_speed_data;
extern char Air_cal_data;
extern char Sol_cal_data,set_sleep_time;
extern uint16_t Air_set_temp_data;
extern uint16_t Sol_set_temp_data;
extern uint16_t Air_real_temp_data, Sol_real_temp_data;
extern uint16_t sol_set_cal_data,air_set_cal_data;
/* General definition */
extern uint8_t init_outtime,  flicker_time, Key_Cycle_Times, main_cycle, Cycle_Times_Error;
extern uint8_t Min_Sum_Air, Max_Sum_Air, Min_Sum_Sol, Max_Sum_Sol, Error_num_Air, Error_num_Sol;
extern uint8_t menu_level,  Second_menu_number, Air_Work_Mode, Sol_Work_Mode;
extern uint8_t Air_Slp_Sum, Air_Run_Sum, Air_Off_Sum, Sol_Off_Sum, Sol_Slp_Sum, Sol_Run_Sum, Ec_Cycle_Times, Ec_End_Times;
extern uint8_t Disp_Com,Sol_Sleep_Time,Time_colck_1;
extern uint16_t Cycle_sleep_time;
extern char First_menu_number;

/* bit definition*/
extern bit error_air_flag, error_sol_flag, start_init_flag, end_init_flag;
extern bit CF_flag, disp_gate, speak_gate, ss_flag, last_state_flag, P_ss_flag, P_ss_bz;
extern bit Updata_Air_Speed_Flag, Key_Ent_flag, Disp_Renew_Flag_Sol, Disp_Renew_Flag_Air;
extern bit Ec_ent_flag, Save_Flag, Air_sleep_flag, Sol_sleep_flag, Disp_Renew_Flag_Fan;
extern bit Close_Sol_Flag, Sleep_Sol_Flag,set_cf_flag;
extern bit Air_restart_flag,Sol_restart_flag;
extern bit Last_Air_Sleep_Mode,Sol_Sleep_Ready_Flag;
/* key definition */
extern bit set_flag,longkey_flag;
extern uint8_t key_out_time;
extern uint16_t air_key_data, sol_key_data;

#endif