#ifndef	__ADZU_H__
#define __ADZU_H__

#define uint8_t unsigned char
#define uint16_t unsigned int
/*
 ADC转换函数：12位AD转换精度＃?５饔檬敝恍枋淙氲テ?机的第几个脚 ，从0开始 返回整型值
*/
uint16_t adc_value_10(uint8_t inp_n);

uint16_t ADC_S(void);
uint16_t ADC_Average_Value(void);//10次ＡＤ求平均值
void ADC_Getvalue(uint8_t inp_n);
	
#endif