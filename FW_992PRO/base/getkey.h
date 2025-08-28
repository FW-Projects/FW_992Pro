#ifndef __getkey_H__
#define __getkey_H__
#define uint8_t unsigned char
#define uint16_t unsigned int
#define key_init 1             // 按键初始化代号
#define key_wobble 2           // 捎抖代号
#define key_press 3            // 有键按下代号
#define keylong_stat 4         // 长按键起始代号
#define key_long_on 5          // 长按键连发代号
#define key_period 6           // 按键释放代号
#define keylong_pepione 50     // 长按键时间常数代号
#define key_continue_period 10 // 按键连发周期常数代号
#define keydown_ent 1          // 单击
#define keyup_ent 2
#define key_long 3
#define key_continue 4 // 长按健
#define key_null 0
#define key_set 1
#define key_air_up 2
#define key_air_down 3
#define key_sol_up 4
#define key_sol_down 5
extern uint8_t inkey_number; // 保存按键代号值
extern uint8_t key_stat;
extern uint16_t key_time;

key_scan(void);
get_key(void);
#endif