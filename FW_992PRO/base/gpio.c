
#include <SN8F5708.H>
#include "gpio.h"

void gpio_init(void)
{
	/* LCD input gpio */
	P0UR = 0x00;
	P0M = 0xff;
	
	P1UR = 0x00;
	P1M = 0xff;

	/* key input gpio */	
	P2UR = 0xff;
	P2M = 0x00;
	P5UR |= 1 << 5;
	P5M &= ~(1 << 5);

	//sol_slp	
	P3UR |= 1 << 3;
	P3M &= ~(1 << 3);

	//air_slp
	P5UR |= 1 << 3;
	P5M &= ~(1 << 3);

	//air_pwm
	P5UR &= ~(1 << 2);
	P5M |= 1 << 2;

	//sol_offon
	P5UR |= 1 << 1;
	P5M &= ~(1 << 1);

	//airhot_pwm
	P5UR &= ~(1 << 0);
	P5M |= 1 << 0;

	//solhot_pwm
	P4UR &= ~(1 << 6);
	P4M |= 1 << 6;

	//air_offon
	P4UR |= 1 << 7;
	P4M &= ~(1 << 7);

	//ec
	P4UR |= 3 << 3;
	P4M &= ~(3 << 3);

	//air_hotad + sol_hotad
	P4CON |= 3 << 1;
	P4UR &= ~(3 << 1);
	P4M &= ~(3 << 1);

	//hot_key
	P5UR &= ~(1 << 4);
	P5M |= 1 << 4;


}