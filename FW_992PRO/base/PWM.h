#ifndef __PWM_H__
#define __PWM_H__

#define uint8_t unsigned char
#define uint16_t unsigned int
/*
 */
void PWM1_init(void);
void PWM2_init(void);
void PWM3_init(void);
void PWM1_out(uint16_t PWM1_d);
void PWM2_out(uint16_t PWM2_d);
void PWM3_out(uint16_t PWM3_d);
void PWM1_Off(void);
void PWM2_Off(void);
void PWM_ALL_OFF(void);
void PWM1_Open(void);
void PWM2_Open(void);
void PWM_ALL_OPEN(void);

#endif