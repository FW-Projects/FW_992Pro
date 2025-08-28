#include <SN8F5708.h>
#include <intrins.h>
#include "adzh_10.h"
#include "PID_operation.h"
#include "ISP.h"
#include "getkey.h"
#include "delay.h"
#include "lcd.h"
#include "tmr.h"
#include "lcd.h"
#include "definition.h"
#include "gpio.h"
#include "PWM.h"
#include "EC11.h"

void Run_Air_Handle(void);
void Run_Sol_Handle(void);
uint16_t Read_Air_Adc();
uint16_t Read_Sol_Adc();
void start_init(void);
void Air_Speed_Handle(void);
void Disp_Handle(void);
void Check_Mode_Handle(void);
void Key_Handle(void);
void Save_Data_Handle(void);
uint16_t EC_Handle(uint16_t in_fan);
uint8_t check_sol_work_mode(uint8_t in_Mode);
uint8_t check_air_work_mode(uint8_t in_mode);
void Check_Error_Air(uint16_t in_err_data);
void Check_Error_Sol(uint16_t in_err_data);
void wd16z8(uint16_t in_16data, uint8_t wn);
uint16_t rd8z16(uint8_t rdin_i);
uint16_t Pid_Control_Air(uint16_t Set_data, uint16_t Cur_data);
uint16_t Pid_Control_Sol(uint16_t Set_dat, uint16_t Cur_dat);
void Disp_Pid_Hot_Sol(uint16_t in_hot);
void Disp_Pid_Hot_Air(uint16_t in_hot);
uint8_t panduan_pid_mode(int pd_aa, pd_ab, in_hot);
void Sleep_Air_Handle(void);
void Sleep_Sol_Handle(void);
void Close_Air_Handle(void);
void Close_Sol_Handle(void);

void main(void)
{
    CKCON = 0x70;
    CLKSEL = 0x07; // Fcpu = 16M
    CLKCMD = 0x69; // Clock switch start
    WDTR = 0x5A;

    gpio_init();
    InitT0();
    PWM1_init();
    PWM2_init();
    PWM3_init();
	AIR_HOT_KEY = 1;
    start_init_flag = 1;
    end_init_flag = 0;
    init_outtime = 0;
    Sol_restart_flag = 1;
    Air_restart_flag = 1;
    ISPpageread2(); // read rom
	P46 = 0;
	P50 = 0;
    while (1)
    {
        WDTR = 0x5A;
        if (start_init_flag == falsh)
        {
            /* 风枪部分 */
            if (Air_Work_Mode == RUN)
            {
                Run_Air_Handle();
            }
            else if (Air_Work_Mode == SLEEP)
            {
                Sleep_Air_Handle();
            }
            else if (Air_Work_Mode == CLOSE)
            {
                Close_Air_Handle();
            }

            /* 烙铁部分 */
            if (Sol_Work_Mode == RUN)
            {
                Run_Sol_Handle();
            }
            else if (Sol_Work_Mode == SLEEP)
            {
                Sleep_Sol_Handle();
            }
            else if (Sol_Work_Mode == CLOSE)
            {
                Close_Sol_Handle();
            }
        }
        else
            start_init(); // 初始化
        switch (main_cycle)
        {
        case 1:
            /* check work mode */
            Check_Mode_Handle();

            break;
        case 2:
            /* Error check*/
#if 0
            Check_Error_Handle();
#endif
            break;
        case 3:
            /* Disp_handle*/
            Disp_Handle();
            break;
        case 4:
            /* AIR_SPEED */
            Air_Speed_Handle();
            break;
        default:main_cycle = 0x00;
            break;
        }
        main_cycle++;
        if (Key_Cycle_Times == 0)
        {
            Key_Cycle_Times = 0x03;
            Key_Handle();
        }
        Air_speed_data = EC_Handle(Air_speed_data);
    }
}

void Sleep_Air_Handle(void)
{
    if (Air_Sleep_Mode == AUTO) // 立刻休眠 100度显示--- 70度关风枪
    {
		Last_Air_Sleep_Mode = AUTO;
		PWM2_Off();
        Air_pid_outdata = 0;
		pid_mode_air = 0;
        AIR_HOT_KEY = 1; // 关
		
        Air_real_temp_data = Read_Air_Adc();
        if (Air_real_temp_data < 100)
        {
            if (Air_real_temp_data < 70)
            {
                PWM3_out(0);
                Disp_Number_Air_Speed = DISP_SLEEP;
                Disp_Renew_Flag_Fan = on;
            }
            Air_sleep_flag = on;
            if (Key_Ent_flag == off)
            {
                Disp_Number_Air = DISP_SLEEP;
                Disp_Renew_Flag_Air = on;
                Disp_Number_Air_Speed = DISP_SLEEP;
                Disp_Renew_Flag_Fan = on;
            }
        }
        else
        {
			PWM3_out(0xff);
            Air_sleep_flag = off;
            if (Key_Ent_flag == off)
            {
                if (CF_flag == 1)
                    Disp_Number_Air = DISP_REALTEMP_Fahrenheit;
                else
                    Disp_Number_Air = DISP_REALTEMP;
                Disp_Renew_Flag_Air = on;
                Disp_Number_Air_Speed = DISP_SLEEPING;
                Disp_Renew_Flag_Fan = on;
            }
        }
    }
    else if (Air_Sleep_Mode == MANUAL) // 不休眠
    {
		if(Last_Air_Sleep_Mode == AUTO)
		{
			pid_mode_air = 0;
			start_init_flag = 1;
		}
		Last_Air_Sleep_Mode = MANUAL;
        Disp_Number_Air_Speed = DISP_WORK;
        Disp_Renew_Flag_Fan = on;
        Run_Air_Handle();
    }
}
void Sleep_Sol_Handle(void)
{
	PWM1_Off();
	Sol_pid_outdata = 0;
	pid_mode_sol = 0;
	Sol_real_temp_data = Read_Sol_Adc();
	if (Sol_real_temp_data < 200) // 休眠
	{
		Sleep_Sol_Flag = on;
		Disp_Number_Sol = DISP_SLEEP;
		Disp_Renew_Flag_Sol = on;
	}
	else
	{
		Sleep_Sol_Flag = off;
		if (Key_Ent_flag == off)
		{
			if (CF_flag == 1)
				Disp_Number_Sol = DISP_REALTEMP_Fahrenheit;
			else
				Disp_Number_Sol = DISP_REALTEMP;
			Disp_Renew_Flag_Sol = on;
		}
	}

}
void Close_Air_Handle(void)
{
    PWM2_Off();
    Air_pid_outdata = 0;
	pid_mode_air = 0;
    AIR_HOT_KEY = 1;
    Air_real_temp_data = Read_Air_Adc();
    if (Air_real_temp_data < 100) // 关风
        PWM3_out(0);
    Close_Sol_Flag = on;
    Disp_Number_Air = DISP_OFF;
    Disp_Number_Air_Speed = DISP_OFF;
    Disp_Renew_Flag_Fan = on;
    Disp_Renew_Flag_Air = on;
    Cycle_Times_Disp_Air = 0x00;
}
void Close_Sol_Handle(void)
{
	PWM1_Off();
    Sol_pid_outdata = 0;
	pid_mode_sol = 0;
    Close_Sol_Flag = on;
    Disp_Number_Sol = DISP_OFF;
    Disp_Renew_Flag_Sol = on;
    Cycle_Times_Disp_Sol = 0x00;

}
void Run_Air_Handle(void)
{
    if (error_air_flag == falsh)
    {
        if (Air_PID_Cycle_Time == 0)
        {
            Air_PID_Cycle_Time = 0x03;
            Air_real_temp_data = Pid_Control_Air(Air_set_temp_data, Air_real_temp_data);
            Disp_Pid_Hot_Air(Air_real_temp_data);
        }
    }
    else
        error_air_data = Read_Air_Adc();
}

void Run_Sol_Handle(void)
{
    if (error_sol_flag == falsh)
    {
        if (Sol_PID_Cycle_Time == 0)
        {
            Sol_PID_Cycle_Time = 0x03;
            Sol_real_temp_data = Pid_Control_Sol(Sol_set_temp_data, Sol_real_temp_data);
            Disp_Pid_Hot_Sol(Sol_real_temp_data);
        }
    }
    else
        error_sol_data = Read_Sol_Adc();
}

void Disp_Pid_Hot_Air(uint16_t in_hot)
{
    if (set_flag == off && Key_Ent_flag == off)
    {
        if (disp_gate == off)
        {
            if (CF_flag == 1)
            {
                Disp_Number_Air = DISP_REALTEMP_Fahrenheit;
            }
            else
            {
                Disp_Number_Air = DISP_REALTEMP;
            }
            Disp_Number_Air_Speed = DISP_WORK;
            Disp_Renew_Flag_Fan = on;
            Disp_Renew_Flag_Air = on;       
		}
        else
        {
            if (pid_hot == panduan_pid_mode(Air_set_temp_data, in_hot, 10))
            {
                if (CF_flag == 1)
                    Disp_Number_Air = DISP_SETTEMP_Fahrenheit;
                else
                    Disp_Number_Air = DISP_SETTEMP;
            }
            else
            {
                if (CF_flag == 1)
                    Disp_Number_Air = DISP_REALTEMP_Fahrenheit;
                else
                    Disp_Number_Air = DISP_REALTEMP;
            }
            Disp_Number_Air_Speed = DISP_WORK;
            Disp_Renew_Flag_Fan = on;
            Disp_Renew_Flag_Air = on;
        }
    }
}

void Disp_Pid_Hot_Sol(uint16_t in_hot)
{
    if (set_flag == off && Key_Ent_flag == off)
    {
        if (disp_gate == off)
        {
            if (CF_flag == 1)
            {
                Disp_Number_Sol = DISP_REALTEMP_Fahrenheit;
            }
            else
            {
                Disp_Number_Sol = DISP_REALTEMP;
            }
            Disp_Renew_Flag_Sol = on;
        }
        else
        {
            if (pid_hot == panduan_pid_mode(Air_set_temp_data, in_hot, 10))
            {
                if (CF_flag == 1)
                    Disp_Number_Sol = DISP_SETTEMP_Fahrenheit;
                else
                    Disp_Number_Sol = DISP_SETTEMP;
            }
            else
            {
                if (CF_flag == 1)
                    Disp_Number_Sol = DISP_REALTEMP_Fahrenheit;
                else
                    Disp_Number_Sol = DISP_REALTEMP;
            }
            Disp_Renew_Flag_Sol = on;
        }
    }
}

uint16_t Pid_Control_Air(uint16_t Set_data, uint16_t Cur_data)
{
    uint16_t hot_data_air = Cur_data;
    hot_data_air = Read_Air_Adc();
    if (pid_mode_air == pid_hot)
    {
        hot_data_air += Air_cal_data;
        Air_pid_outdata = PID_operation_Air(Set_data, hot_data_air);
    }
    else
    {
        pid_mode_air = panduan_pid_mode(Set_data, hot_data_air, 20);
        if (pid_mode_air == add_hot)
            Air_pid_outdata = 5000;
        else if (pid_mode_air == sub_hot)
            Air_pid_outdata = 0;
    }
    return hot_data_air;
}

uint16_t Pid_Control_Sol(uint16_t Set_dat, uint16_t Cur_dat)
{
    uint16_t hot_data_sol = Cur_dat;
    hot_data_sol = Read_Sol_Adc();
    if (pid_mode_sol == pid_hot)
    {
        hot_data_sol += Sol_cal_data;
        Sol_pid_outdata = PID_operation_Sol(Set_dat, hot_data_sol);
    }
    else
    {
        pid_mode_sol = panduan_pid_mode(Set_dat, hot_data_sol, 20);
        if (pid_mode_sol == add_hot)
            Sol_pid_outdata = 5000;
        else if (pid_mode_sol == sub_hot)
            Sol_pid_outdata = 0;
    }
    return hot_data_sol;
}

uint8_t panduan_pid_mode(int pd_aa, pd_ab, in_hot)
{
    int pad_coe;
    pad_coe = (pd_aa - pd_ab);
    if (pad_coe > in_hot)
        return add_hot;
    else if (pad_coe < -in_hot)
        return sub_hot;
    else
        return pid_hot;
}

uint16_t Read_Air_Adc()
{
    uint16_t out_data_air = 0x00;
    PWM2_Off();
    Delay_X10Us(20);
    out_data_air = adc_value_10(1);
    PWM2_Open();
    return out_data_air;
}

uint16_t Read_Sol_Adc()
{
    uint16_t out_data_sol = 0x00;
    PWM1_Off();
    Delay_X10Us(20);
    out_data_sol = adc_value_10(2);
    PWM1_Open();
    return out_data_sol;
}

uint16_t EC_Handle(uint16_t in_fan)
{
    uint8_t fan_ec = 0;
    uint16_t ec_outdata = in_fan;
    fan_ec = ec11_ve();
    if (fan_ec == 'Y')
    {
        Ec_ent_flag = on;
        if (Ec_Cycle_Times != 0)
            ec_outdata += 10;
        else
            ec_outdata++;
        if (ec_outdata > 99)
            ec_outdata = 99;
        Updata_Air_Speed_Flag = on; // 风量更新
        Disp_Number_Air_Speed = DISP_WORK;
        Disp_Renew_Flag_Fan = on;
        Ec_End_Times = 0x20;
        Ec_Cycle_Times = 0x20;
    }
    else if (fan_ec == 'Z')
    {
        Ec_ent_flag = on;
        if (Ec_Cycle_Times != 0)
            ec_outdata -= 10;
        else
            ec_outdata--;
        if (ec_outdata < 10)
            ec_outdata = 10;
        Updata_Air_Speed_Flag = on; // 风量更新
        Disp_Number_Air_Speed = DISP_WORK;
        Disp_Renew_Flag_Fan = on;
        Ec_End_Times = 0x20;
        Ec_Cycle_Times = 0x20;
    }
    else if (Ec_ent_flag == on && Ec_End_Times == 0x00)
    {
        Ec_ent_flag = off;
        if (Air_sleep_flag == on)
            Disp_Number_Air_Speed = DISP_SLEEP;
        else
            Disp_Number_Air_Speed = DISP_WORK;
        Disp_Renew_Flag_Fan = on;
        Save_Data_Handle();
    }
    return ec_outdata;
}

void Disp_Handle(void)
{
    if (set_flag == on)
    {
        Disp_Set_Handle( First_menu_number);
    }
    else
    {
        if (Disp_Renew_Flag_Air == on && Cycle_Times_Disp_Air == 0x00)
        {
            Disp_Renew_Flag_Air = off;
            Cycle_Times_Disp_Air = 0x0a;
            Disp_Air_Handle(Disp_Number_Air);
        }
        if (Disp_Renew_Flag_Sol == on && Cycle_Times_Disp_Sol == 0x00)
        {
            Disp_Renew_Flag_Sol = off;
            Cycle_Times_Disp_Sol = 0x0a;
            Disp_Sol_Handle(Disp_Number_Sol);
        }
        if (Disp_Renew_Flag_Fan == on)
        {
            Disp_Renew_Flag_Fan = off;
            Disp_Fan_Handle(Disp_Number_Air_Speed);
        }
    }
}

void Check_Mode_Handle(void)
{
    Air_Work_Mode = check_air_work_mode(Air_Work_Mode);
    Sol_Work_Mode = check_sol_work_mode(Sol_Work_Mode);
}

uint8_t check_air_work_mode(uint8_t in_mode)
{
    uint8_t out_mode = in_mode;
    if (Air_Check_OffOn == 1)
    {
        // 风枪开关检测
        Air_Off_Sum++;
        Air_Run_Sum = 0;
        Air_Slp_Sum = 0;
        if (Air_Off_Sum > 3)
        {
            Air_Off_Sum = 0;
            out_mode = CLOSE;
        }
    }
    else if (Air_Check_Sleep == 0)
    {
        // 风枪睡眠检测
        Air_Slp_Sum++;
        Air_Run_Sum = 0;
        Air_Off_Sum = 0;
        if (Air_Slp_Sum > 3)
        {
            Air_Slp_Sum = 0;
            out_mode = SLEEP;
        }
    }
    else if (Air_Check_Sleep == 1)
    {
        // 风枪运行检测
        Air_Run_Sum++;
        Air_Off_Sum = 0;
        Air_Slp_Sum = 0;
        if (Air_Run_Sum > 1)
        {
            if (out_mode == CLOSE || out_mode == SLEEP)
            {
                start_init_flag = 1;
                Air_restart_flag = 1;
            }
            Air_Run_Sum = 0;
            out_mode = RUN;
        }
    }
    return out_mode;
}

uint8_t check_sol_work_mode(uint8_t in_Mode)
{
    uint8_t out_Mode = in_Mode;
    if (Sol_Check_OffOn == 1)
    {
        // 烙铁开关检测
        Sol_Off_Sum++;
        Sol_Run_Sum = 0;
        Sol_Slp_Sum = 0;
        if (Sol_Off_Sum > 3)
        {
            Sol_Off_Sum = 0;
            out_Mode = CLOSE;
			Sol_Sleep_Ready_Flag = off;
        }
    }
	else
	{
		if(out_Mode == CLOSE )
		{
			start_init_flag = 1;
			Sol_restart_flag = 1;
		}
		if(set_sleep_time == 0x00)  //不休眠
		{
			out_Mode = RUN;
			Sol_Off_Sum = 0;
			Sol_Slp_Sum = 0;
			Sol_Run_Sum = 0;
			Sol_Sleep_Ready_Flag = off;
		}
		else
		{
			if(Sol_Sleep_Time != 0x00)  //工作才要休眠
			{
				out_Mode = RUN;
				if (Sol_Check_Sleep == 1)  //休眠
				{
					Sol_Slp_Sum++;
					Sol_Off_Sum = 0;
					if(Sol_Slp_Sum > 10)
					{
						Sol_Sleep_Ready_Flag = on;  //烙铁休眠准备标志 开始休眠计时
						Sol_Slp_Sum = 0;
						Sol_Run_Sum = 0;
					}
				}
			}
			else   //休眠才要唤醒
			{
				out_Mode = SLEEP;
				if (Sol_Check_Sleep == 0)  //唤醒
				{
					Sol_Run_Sum++;
					Sol_Off_Sum = 0;
					Sol_Slp_Sum = 0;
//					if(Sol_Run_Sum > 1)
//					{
						Sol_Sleep_Time = set_sleep_time;
						out_Mode = RUN;
						Sol_Run_Sum = 0;
						Sol_Sleep_Ready_Flag = off;
//					}
				}
			}
		}
	}
    return out_Mode;
}

void Check_Error_Handle(void)
{
    Check_Error_Air(error_air_data);
    Check_Error_Sol(error_sol_data);
}

void Check_Error_Air(uint16_t in_err_data)
{
    // 风枪错误检测
    if (Cycle_Times_Error == 0)
    {
        if ((end_init_flag == 0) && (in_err_data < 70)) //????
        {
            if (Min_Sum_Air == 0)
            {
                error_air_flag = 1;
				PWM2_Off();
                Air_pid_outdata = 0;
                AIR_HOT_KEY = 1;
                Error_num_Air = 1;
                if (Key_Ent_flag == off)
                {
                    Disp_Number_Air = DISP_ERR;
                    Disp_Renew_Flag_Air = on;
                }
                Min_Sum_Air = 20;
            }
            else
                Min_Sum_Air--;
        }
        else if (in_err_data > 580) //????
        {
            if (Max_Sum_Air == 0)
            {
                error_air_flag = 1;
                AIR_HOT_KEY = 1; // 先停止加热 温度下降后再关风
                PWM2_Off();
                Air_pid_outdata = 0;
                Error_num_Air = 2;
                if (Key_Ent_flag == off)
                {
                    Disp_Number_Air = DISP_ERR;
                    Disp_Renew_Flag_Air = on;
                }
                Max_Sum_Air = 3;
            }
            else
                Max_Sum_Air--;
        }
        Cycle_Times_Error = 0x02;
    }
}

void Check_Error_Sol(uint16_t in_err_data)
{
    // 烙铁错误检测
    if (Cycle_Times_Error == 0)
    {
        if ((end_init_flag == 0) && (in_err_data < 70)) //????
        {
            if (Min_Sum_Sol == 0)
            {
                error_sol_flag = 1;
                PWM1_Off();
                Sol_pid_outdata = 0;
                Error_num_Sol = 1;
                if (Key_Ent_flag == off)
                {
                    Disp_Number_Sol = DISP_ERR;
                    Disp_Renew_Flag_Sol = on;
                }
                Min_Sum_Sol = 20;
            }
            else
                Min_Sum_Sol--;
        }
        else if (in_err_data > 580) //????
        {
            if (Max_Sum_Sol == 0)
            {
                error_sol_flag = 1;
                PWM1_Off();
                Sol_pid_outdata = 0;
                Error_num_Sol = 2;
                if (Key_Ent_flag == off)
                {
                    Disp_Number_Sol = DISP_ERR;
                    Disp_Renew_Flag_Sol = on;
                }
                Max_Sum_Sol = 3;
            }
            else
                Max_Sum_Sol--;
        }
        Cycle_Times_Error = 0x02;
    }
}

void Air_Speed_Handle(void)
{
    uint16_t pwm_out = 0x00;
	if (Updata_Air_Speed_Flag == 1)
	{
		Updata_Air_Speed_Flag = 0;
		pwm_out = Air_speed_data * 2.2 + 30;
		if (pwm_out > 0xff)
			pwm_out = 0xff;
		PWM3_out(pwm_out);
	}
}
void start_init(void)
{
    if (start_init_flag == 1 && end_init_flag == 0)
    {
        PWM_ALL_OFF(); // 关闭全部PWM通道
		PWM3_out(66);
        PID_Init_Sol();
        PID_Init_Air();
        CF_flag = u8_data[0];
        CF_flag = 0;
        set_cf_flag = CF_flag;
        disp_gate = u8_data[2];
//        disp_gate = 0;
		speak_gate = u8_data[3];
        Air_speed_data =  rd8z16(14);
        if (Air_speed_data > 99)
            Air_speed_data = 50;
        else if (Air_speed_data < 0)
            Air_speed_data = 50;

        Air_cal_data = rd8z16(4);
        Sol_cal_data = rd8z16(6);
        if (Air_cal_data > 99)
            Air_cal_data = 0;
        else if (Air_cal_data < 0)
            Air_cal_data = 0;
        if (Sol_cal_data > 99)
            Sol_cal_data = 0;
        else if (Sol_cal_data < 0)
            Sol_cal_data = 0;

        Air_set_temp_data = rd8z16(8);
        Sol_set_temp_data = rd8z16(10);
        if (Air_set_temp_data > 500)
            Air_set_temp_data = 300;
        else if (Air_set_temp_data < 0)
            Air_set_temp_data = 300;
        if (Sol_set_temp_data > 500)
            Sol_set_temp_data = 300;
        else if (Sol_set_temp_data < 0)
            Sol_set_temp_data = 300;

        if (CF_flag == 1)
        {
            sol_key_data = ((Sol_set_temp_data * 9 / 5) + 32);
            air_key_data = ((Air_set_temp_data * 9 / 5) + 32);
        }
        else
        {
            sol_key_data = Sol_set_temp_data;
            air_key_data = Air_set_temp_data;
        }
        sol_set_cal_data = sol_key_data;
        air_set_cal_data = air_key_data;
        set_sleep_time = rd8z16(12);
        if (set_sleep_time < 0 || set_sleep_time > 99)
            set_sleep_time = 1;  //默认五分钟休眠
        Sol_Sleep_Time = set_sleep_time;

		pid_mode_air = 0;
		pid_mode_sol = 0;
        Error_num_Air = 0; // 风枪错误代码
        Error_num_Sol = 0; // 烙铁错误代码
        Error_num_Air = 0;
        Error_num_Sol = 0;
        Min_Sum_Air = 20;
        Max_Sum_Air = 4;
        Min_Sum_Sol = 20;
        Max_Sum_Sol = 4;
        key_out_time = 0;
        menu_level = 1;
        First_menu_number = 1;
        Second_menu_number = 1;
		Sol_Sleep_Ready_Flag = 0;
		Check_Mode_Handle();
		Updata_Air_Speed_Flag = on;
        if (Air_restart_flag == on)
        {
//            Air_Work_Mode = RUN; // 风枪工作模式
            Air_restart_flag = off;
            
            Disp_Renew_Flag_Air = on;
            Cycle_Times_Disp_Air = 0x00;
            Disp_Renew_Flag_Fan = on;
            Disp_Number_Air_Speed = DISP_WORK;
            Disp_Number_Air = DISP_SETTEMP;
        }
        if (Sol_restart_flag == on)
        {
//            Sol_Work_Mode = RUN; // 烙铁工作模式
            Sol_restart_flag = off;
            Disp_Renew_Flag_Sol = on;
            Cycle_Times_Disp_Sol = 0x00;
            Disp_Number_Sol = DISP_SETTEMP;
        }
		if(Air_Work_Mode == CLOSE)
		{
			Disp_Number_Air = DISP_OFF;
			 Disp_Number_Air_Speed = DISP_OFF;
		}
		if(Sol_Work_Mode == CLOSE)
		{
			Disp_Number_Sol = DISP_OFF;
		}
        set_flag = 0;
        AIR_HOT_KEY = 1; // 不加热
        init_outtime = 0x0a;
        end_init_flag = 1;
    }
    else if (end_init_flag == 1 && init_outtime == 0x00)
    {
        PWM_ALL_OPEN();
        start_init_flag = 0;
        end_init_flag = 0;
        AIR_HOT_KEY = 0; // 加热
    }
}

void Key_Handle(void)
{
    uint8_t key_value;
    key_value = get_key();
    if (inkey_number == key_set) //  设置界面
    {
        switch (key_value)
        {
        case (keydown_ent):
            break;
        case (keyup_ent):
        {
            Key_Ent_flag = on;
            inkey_number = key_null;
            longkey_flag = 0;
        }
        break;
        case (key_long):
            break;
        case (key_continue):
        {
            if (longkey_flag == 0)
            {
                longkey_flag = 1;
                Key_Ent_flag = on;
                if (set_flag == 0) // 打开设置界面
                {
                    set_flag = 1;
                    menu_level = 1;         // 一级菜单
                    First_menu_number = 0;  // 一级菜单序号
                    Second_menu_number = 0; // 二级菜单序号
                    key_out_time = 100;     // 设置界面退出时间
                }
                else
                {
                    set_flag = 0; // 关闭设置界面
                    key_out_time = 0;
                }
            }
        }
        break;
        default:
            break;
        }
    }
    else if (inkey_number == key_air_up)
    {
        switch (key_value)
        {
        case (keydown_ent):
            break;
        case (keyup_ent):
        {
            Key_Ent_flag = on;
            if (set_flag == 0) // 非设置界面
            {
                air_key_data++;
                if (air_key_data > 500)
                    air_key_data = 500;
                Disp_Number_Air = DISP_SETTEMP;
                Disp_Renew_Flag_Air = on;
                Cycle_Times_Disp_Air = 0x00;
                key_out_time = 0x30;
            }
            else
            {
                First_menu_number++;
                if (First_menu_number > 6)
                    First_menu_number = 0;
				if(First_menu_number == 1)
				{
					if(CF_flag == 1 || set_cf_flag == 1)
						air_set_cal_data = (Air_set_temp_data * 9 / 5) + 32;
					else if(CF_flag == 0 || set_cf_flag == 0)
						air_set_cal_data = Air_set_temp_data;
				}
				if(First_menu_number == 2)
				{
					if(CF_flag == 1 || set_cf_flag == 1)
						sol_set_cal_data = (Sol_set_temp_data * 9 / 5) + 32;
					else if(CF_flag == 0 || set_cf_flag == 0)
						sol_set_cal_data = Sol_set_temp_data;
				}
                key_out_time = 0x50;
            }
            inkey_number = key_null;
        }
        break;
        case (key_long):
            break;
        case (key_continue):
        {
            Key_Ent_flag = on;
            if (set_flag == 0) // 非设置界面
            {
                air_key_data += 10;
                if (air_key_data > 500)
                    air_key_data = 500;
                Disp_Number_Air = DISP_SETTEMP;
                Disp_Renew_Flag_Air = on;
                Cycle_Times_Disp_Air = 0x00;
                key_out_time = 0x30;
            }
        }
        break;
        default:
            break;
        }
    }
    else if (inkey_number == key_air_down)
    {
        switch (key_value)
        {
        case (keydown_ent):
            break;
        case (keyup_ent):
        {
            Key_Ent_flag = on;
            if (set_flag == 0) // 非设置界面
            {
                air_key_data--;
                if (air_key_data < 100)
                    air_key_data = 100;
                Disp_Number_Air = DISP_SETTEMP;
                Disp_Renew_Flag_Air = on;
                Cycle_Times_Disp_Air = 0x00;
                key_out_time = 0x30;
            }
            else
            {
                First_menu_number--;
                if (First_menu_number < 0)
                    First_menu_number = 6;
				if(First_menu_number == 1)
				{
					if(CF_flag == 1 || set_cf_flag == 1)
						air_set_cal_data = (Air_set_temp_data * 9 / 5) + 32;
					else if(CF_flag == 0 || set_cf_flag == 0)
						air_set_cal_data = Air_set_temp_data;
				}
				if(First_menu_number == 2)
				{
					if(CF_flag == 1 || set_cf_flag == 1)
						sol_set_cal_data = (Sol_set_temp_data * 9 / 5) + 32;
					else if(CF_flag == 0 || set_cf_flag == 0)
						sol_set_cal_data = Sol_set_temp_data;
				}
                key_out_time = 0x50;
            }
            inkey_number = key_null;
        }
        break;
        case (key_long):
            break;
        case (key_continue):
        {

            Key_Ent_flag = on;
            if (set_flag == 0) // 非设置界面
            {
                air_key_data -= 10;
                if (air_key_data < 100)
                    air_key_data = 100;
                Disp_Number_Air = DISP_SETTEMP;
                Disp_Renew_Flag_Air = on;
                Cycle_Times_Disp_Air = 0x00;
                key_out_time = 0x30;
            }
        }
        break;
        default:
            break;
        }
    }
    else if (inkey_number == key_sol_up)
    {
        switch (key_value)
        {
        case (keydown_ent):
            break;
        case (keyup_ent):
        {

            Key_Ent_flag = on;
            if (set_flag == 0) // 非设置界面
            {
                sol_key_data++;
                if (CF_flag == 0)
                {
                    if (sol_key_data > 450)
                        sol_key_data = 450;
                }
                else
                {
                    if (sol_key_data > 842)
                        sol_key_data = 842;
                }
                Disp_Number_Sol = DISP_SETTEMP;
                Disp_Renew_Flag_Sol = on;
                Cycle_Times_Disp_Sol = 0x00;
                key_out_time = 0x30;
            }
            else
            {
                switch (First_menu_number)
                {
                case OUT:
                    /* code */
                    break;
                case AIR_COMPENSATION:
                    air_set_cal_data++;
                    if (CF_flag == 1)
                    {
                        if (air_set_cal_data > 999)
                            air_set_cal_data = 999;
                    }
                    else
                    {
                        if (air_set_cal_data > 600)
                            air_set_cal_data = 600;
                    }
                    break;
                case SOL_COMPENSATION:
                    sol_set_cal_data++;
                    if (CF_flag == 1)
                    {
                        if (sol_set_cal_data > 999)
                            sol_set_cal_data = 999;
                    }
                    else
                    {
                        if (sol_set_cal_data > 600)
                            sol_set_cal_data = 600;
                    }
                    break;
                case CF_MODE:
                    set_cf_flag = ~set_cf_flag;
                    break;
                case DISPLAY_MODE:
                    disp_gate = ~disp_gate;
                    break;
				case SPEAK_MODE:
					speak_gate = ~speak_gate;
				break;
                case SLEEP_TIME:
                    set_sleep_time++;
                    if (set_sleep_time > 99)
                        set_sleep_time = 00;
                    break;
                default:
                    break;
                }
                key_out_time = 0x50;
            }
            inkey_number = key_null;
        }
        break;
        case (key_long):
            break;
        case (key_continue):
        {
            Key_Ent_flag = on;
            if (set_flag == 0)
            {
                sol_key_data += 5;
                if (CF_flag == 0)
                {
                    if (sol_key_data > 450)
                        sol_key_data = 450;
                }
                else
                {
                    if (sol_key_data > 842)
                        sol_key_data = 842;
                }
                Disp_Number_Sol = DISP_SETTEMP;
                Disp_Renew_Flag_Sol = on;
                Cycle_Times_Disp_Sol = 0x00;
                key_out_time = 0x30;
            }
            else
            {
                switch (First_menu_number)
                {
                case AIR_COMPENSATION:
                    air_set_cal_data += 5;
                    if (CF_flag == 1)
                    {
                        if (air_set_cal_data > 1112)
                            air_set_cal_data = 1112;
                    }
                    else
                    {
                        if (air_set_cal_data > 600)
                            air_set_cal_data = 600;
                    }
                    break;
                case SOL_COMPENSATION:
                    sol_set_cal_data += 5;
                    if (CF_flag == 1)
                    {
                        if (sol_set_cal_data > 1022)
                            sol_set_cal_data = 1022;
                    }
                    else
                    {
                        if (sol_set_cal_data > 550)
                            sol_set_cal_data = 550;
                    }
                    break;
                case SLEEP_TIME:
                    set_sleep_time += 5;
                    if (set_sleep_time > 99)
                        set_sleep_time = 00;
                    break;
                default:
                    break;
                }
                key_out_time = 0x50;
            }
        }
        break;
        default:
            break;
        }
    }
    else if (inkey_number == key_sol_down)
    {
        switch (key_value)
        {
        case (keydown_ent):
            break;
        case (keyup_ent):
        {

            Key_Ent_flag = on;
            if (set_flag == 0) // 非设置界面
            {
                sol_key_data--;
                if (CF_flag == 0)
                {
                    if (sol_key_data < 200)
                        sol_key_data = 200;
                }
                else
                {
                    if (sol_key_data < 392)
                        sol_key_data = 392;
                }
                Disp_Number_Sol = DISP_SETTEMP;
                Disp_Renew_Flag_Sol = on;
                Cycle_Times_Disp_Sol = 0x00;
                key_out_time = 0x30;
            }
            else
            {
                switch (First_menu_number)
                {
                case OUT:
                    /* code */
                    break;
                case AIR_COMPENSATION:
                    air_set_cal_data--;
                    if (CF_flag == 1)
                    {
                        if (air_set_cal_data < 34)
                            air_set_cal_data = 34;
                    }
                    else
                    {
                        if (air_set_cal_data < 1)
                            air_set_cal_data = 1;
                    }
                    break;
                case SOL_COMPENSATION:
                    sol_set_cal_data--;
                    if (CF_flag == 1)
                    {
                        if (sol_set_cal_data > 212)
                            sol_set_cal_data = 212;
                    }
                    else
                    {
                        if (sol_set_cal_data > 100)
                            sol_set_cal_data = 100;
                    }
                    break;
                case CF_MODE:
                    set_cf_flag = ~set_cf_flag;
					
                    break;
                case DISPLAY_MODE:
                    disp_gate = ~disp_gate;
                    break;
				case SPEAK_MODE:
					speak_gate = ~speak_gate;
				break;
                case SLEEP_TIME:
                    set_sleep_time--;
                    if (set_sleep_time < 0)
                        set_sleep_time = 0;
                    break;
                default:
                    break;
                }
                key_out_time = 0x50;
            }

            inkey_number = key_null;
        }
        break;
        case (key_long):
            break;
        case (key_continue):
        {
            Key_Ent_flag = on;
            if (set_flag == 0)
            {
                sol_key_data -= 5;
                if (CF_flag == 0)
                {
                    if (sol_key_data < 200)
                        sol_key_data = 200;
                }
                else
                {
                    if (sol_key_data < 392)
                        sol_key_data = 392;
                }
                Disp_Number_Sol = DISP_SETTEMP;
                Disp_Renew_Flag_Sol = on;
                Cycle_Times_Disp_Sol = 0x00;
                key_out_time = 0x30;
            }
            else
            {
                switch (First_menu_number)
                {
                case AIR_COMPENSATION:
                    air_set_cal_data -= 5;
                    if (CF_flag == 1)
                    {
                        if (air_set_cal_data < 34)
                            air_set_cal_data = 34;
                    }
                    else
                    {
                        if (air_set_cal_data < 1)
                            air_set_cal_data = 1;
                    }
                    break;
                case SOL_COMPENSATION:
                    sol_set_cal_data -= 5;
                    if (CF_flag == 1)
                    {
                        if (sol_set_cal_data < 212)
                            sol_set_cal_data = 212;
                    }
                    else
                    {
                        if (sol_set_cal_data < 100)
                            sol_set_cal_data = 100;
                    }
                    break;
                case SLEEP_TIME:
                    set_sleep_time -= 5;
                    if (set_sleep_time < 0)
                        set_sleep_time = 00;
                    break;
                default:
                    break;
                }
                key_out_time = 0x50;
            }
        }
        break;
        default:
            break;
        }
    }
    else if (Key_Ent_flag == on && key_out_time == 0x00)
    {
        if (set_flag == 1)
        {
            set_flag = 0;
			CF_flag = set_cf_flag;
            if (CF_flag == 1)
            {
				sol_key_data = (Sol_set_temp_data * 9 / 5) + 32;
				air_key_data = (Air_set_temp_data * 9 / 5) + 32;
                Air_cal_data = Air_set_temp_data - (5 * (air_set_cal_data - 32) / 9);
                Sol_cal_data = Sol_set_temp_data - (5 * (sol_set_cal_data - 32) / 9);
            }
            else
            {
				sol_key_data = Sol_set_temp_data;
				air_key_data = Air_set_temp_data;
                Air_cal_data = Air_set_temp_data - air_set_cal_data;
                Sol_cal_data = Sol_set_temp_data - sol_set_cal_data;
            }
            
        }
		else 
		{
			pid_mode_air = 0;
			pid_mode_sol = 0;
			PID_Init_Air();
			PID_Init_Sol();
		}
		if(CF_flag == 1)
		{
			Sol_set_temp_data = (5 * (sol_key_data - 32) / 9);
			Air_set_temp_data = (5 * (air_key_data - 32) / 9);
		}
		else 
		{
			 Sol_set_temp_data = sol_key_data;
			 Air_set_temp_data = air_key_data;
		}
        Disp_Renew_Flag_Air = on;
        Cycle_Times_Disp_Air = 0x00;
        Disp_Renew_Flag_Sol = on;
        Cycle_Times_Disp_Sol = 0x00;
        Disp_Renew_Flag_Fan = on;
        Save_Data_Handle();
        Key_Ent_flag = off;
    }
}

void Save_Data_Handle(void)
{
    u8_data[0] = CF_flag;
    u8_data[2] = disp_gate;
	u8_data[3] = speak_gate;
    wd16z8(Air_cal_data, 4);
    wd16z8(Sol_cal_data, 6);
    wd16z8(Air_set_temp_data, 8);
    wd16z8(Sol_set_temp_data, 10);
    wd16z8(set_sleep_time, 12);
	wd16z8(Air_speed_data,14);
    ISPpagewrite(u8_data);
}

void wd16z8(uint16_t in_16data, uint8_t wn) // 8?????:
{
    u8_data[wn] = in_16data >> 8;
    u8_data[wn + 1] = in_16data;
}

uint16_t rd8z16(uint8_t rdin_i)
{
    uint16_t bit16_data = 0;
    bit16_data = u8_data[rdin_i];
    bit16_data <<= 8;
    bit16_data |= u8_data[rdin_i + 1];
    return (bit16_data);
}