#ifndef	__ADZU_H__
#define __ADZU_H__

#define uint8_t unsigned char
#define uint16_t unsigned int
/*
 ADCת��������12λADת�����ȣ�?�����ʱֻ�����뵥�?���ĵڼ����� ����0��ʼ ��������ֵ
*/
uint16_t adc_value_10(uint8_t inp_n);

uint16_t ADC_S(void);
uint16_t ADC_Average_Value(void);//10�Σ�����ƽ��ֵ
void ADC_Getvalue(uint8_t inp_n);
	
#endif