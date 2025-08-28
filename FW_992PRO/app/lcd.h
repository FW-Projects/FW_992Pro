#ifndef __LCD_H__
#define __LCD_H__
#include <SN8F5708.H>

#define uint8_t unsigned char
#define uint16_t unsigned int

void Disp_Air_Handle(uint8_t in_disp_num);
void Disp_Sol_Handle(uint8_t in_disp_num);
void Disp_Fan_Handle(uint8_t in_disp_num);
void Disp_Set_Handle(uint8_t in_First_menu_number);
void led_out(uint16_t led_data);

#endif