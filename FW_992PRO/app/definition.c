#include "definition.h"

/* display definition */
uint8_t Disp_Number_Air = 0, Disp_Number_Sol = 0, Disp_Number_Air_Speed = 0, Cycle_Times_Disp_Air = 0, Cycle_Times_Disp_Sol = 0;
uint8_t Air_bai = 0, Air_shi = 0, Air_ge = 0;
uint8_t Sol_bai = 0, Sol_shi = 0, Sol_ge = 0;
uint8_t Air_speed_shi = 0, Air_speed_ge = 0;

/* PID definition */
uint8_t Air_PID_Cycle_Time = 0, Sol_PID_Cycle_Time = 0;
uint16_t Air_pid_outdata = 0, Sol_pid_outdata = 0;
uint8_t pid_mode_air = 0,pid_mode_sol = 0;
/* data definition*/
uint16_t error_air_data = 0, error_sol_data = 0, Air_speed_data = 0, Air_set_temp_data = 0, Sol_set_temp_data = 0;
uint16_t Air_real_temp_data = 0, Sol_real_temp_data = 0,sol_set_cal_data = 0,air_set_cal_data = 0;
char Air_cal_data = 0, Sol_cal_data = 0,set_sleep_time = 0;
/* General definition */
uint8_t init_outtime = 0,  flicker_time = 0, Key_Cycle_Times = 0, main_cycle = 0, Cycle_Times_Error = 0;
uint8_t Min_Sum_Air = 0, Max_Sum_Air = 0, Min_Sum_Sol = 0, Max_Sum_Sol = 0, Error_num_Air = 0, Error_num_Sol = 0;
uint8_t menu_level = 0,  Second_menu_number = 0, Air_Work_Mode = 0, Sol_Work_Mode = 0;
uint8_t Air_Slp_Sum = 0, Air_Run_Sum = 0, Air_Off_Sum = 0, Sol_Off_Sum = 0, Sol_Slp_Sum = 0, Sol_Run_Sum = 0, Ec_Cycle_Times = 0, Ec_End_Times = 0;
uint8_t Disp_Com = 0,Sol_Sleep_Time = 0,Time_colck_1 = 0;
uint16_t Cycle_sleep_time = 0x7B56;
char First_menu_number = 0;
/* bit definition*/
bit error_air_flag = falsh, error_sol_flag = falsh, start_init_flag = falsh, end_init_flag = 0;
bit CF_flag = 0, disp_gate = 0, speak_gate = 0, ss_flag = 0, last_state_flag = 0, P_ss_flag = 1, P_ss_bz = 0;
bit Updata_Air_Speed_Flag = 0, Key_Ent_flag = 0, Disp_Renew_Flag_Sol = 0, Disp_Renew_Flag_Air = 0;
bit Ec_ent_flag = 0, Save_Flag = 0, Air_sleep_flag = 0, Sol_sleep_flag = 0, Disp_Renew_Flag_Fan = 0;
bit Close_Sol_Flag = 0, Sleep_Sol_Flag = 0,Last_Air_Sleep_Mode = 0,Sol_Sleep_Ready_Flag = 0;
bit Air_restart_flag = 0,Sol_restart_flag = 0,set_cf_flag = 0;
/* key_definition */
bit set_flag = 0,longkey_flag = 0;
uint8_t key_out_time = 0;
uint16_t air_key_data = 0, sol_key_data = 0;