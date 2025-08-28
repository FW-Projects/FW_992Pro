#ifndef	__DELAY_H__
#define __DELAY_H__


#define uint8_t unsigned char
#define uint16_t  unsigned int 

//void Delay_10Us(void);
void Delay_X10Us(uint16_t Time);//10us
void Delay_Ms(uint16_t Time);
void Delay_S(uint16_t Time);
void delay_us(uint16_t xms);

#endif