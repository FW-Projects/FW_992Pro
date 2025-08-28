#ifndef _ISP_h_
#define _ISP_h_
#include <intrins.h> // for _nop_
#include <absacc.h>
#include <SN8F5708.H>

extern idata uint16_t r_read_rom_addr;
extern idata uint8_t u8_data[32];

void Systeminit(void);
void ISPinit(void);
void ISPpagewrite(uint8_t *r_ram_address);
void ISPpageread2(void);
#endif