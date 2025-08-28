#ifndef	__PID_OPERATION_H__
#define __PID_OPERATION_H__


#define uint8_t unsigned char
#define uint16_t unsigned int
void PID_Init_Sol(void);
void PID_Init_Air(void);
uint16_t PID_operation_Air(uint16_t Set_data,uint16_t Cur_data);
uint16_t PID_operation_Sol(uint16_t Set_dat,uint16_t Cur_dat);

#endif