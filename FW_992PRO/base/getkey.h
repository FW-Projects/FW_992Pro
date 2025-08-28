#ifndef __getkey_H__
#define __getkey_H__
#define uint8_t unsigned char
#define uint16_t unsigned int
#define key_init 1             // ������ʼ������
#define key_wobble 2           // �Ӷ�����
#define key_press 3            // �м����´���
#define keylong_stat 4         // ��������ʼ����
#define key_long_on 5          // ��������������
#define key_period 6           // �����ͷŴ���
#define keylong_pepione 50     // ������ʱ�䳣������
#define key_continue_period 10 // �����������ڳ�������
#define keydown_ent 1          // ����
#define keyup_ent 2
#define key_long 3
#define key_continue 4 // ������
#define key_null 0
#define key_set 1
#define key_air_up 2
#define key_air_down 3
#define key_sol_up 4
#define key_sol_down 5
extern uint8_t inkey_number; // ���水������ֵ
extern uint8_t key_stat;
extern uint16_t key_time;

key_scan(void);
get_key(void);
#endif