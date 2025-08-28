#include <stdio.h>
#include <SN8F5708.H>
#include <adzh_10.h>
#define ADCEn 0x80
#define EADC (0 << 3) // �ر�ADC�ж�
#define ADCStart 0xc0
#define ad_flag 0x50
#define ClearEOC 0x8f
#define uint8_t unsigned char
#define uint16_t unsigned int
uint16_t AD_Buffer_Array[10] = {0};
uint16_t ADCBuffer = 0x0000;
uint16_t ad10_data[10] = {0};
uint16_t ADC_S(void);

uint16_t ADC_S(void)
{
	uint16_t ADCBuffer = 0;
	ADM |= ADCStart; // ת����ʼ
	while (ADM & ad_flag)
		; // �ȴ�����
	ADCBuffer = (ADB << 4) + (ADR & 0x0F);
	ADCBuffer >>= 2;
	ADM &= ClearEOC;
	return (ADCBuffer);
}

//void ADC_Getvalue(uint8_t inp_n)
//{
//	uint8_t adn = 0;
//	ADM = ADCEn | inp_n;
//	ADR = 0x60;	  // 4M   00 = Fosc/16;01 = Fosc/8;10 = Fosc/1;11 = Fosc/2?
//	VREFH = 0x07; ///*IEN0 |= 0x80; //enable global interrupt*/
//	IEN2 |= EADC; // �ر��ж�ad
//	for (adn = 0; adn < 10; adn++)
//		ad10_data[adn] = ADC_S();
//}

//uint16_t ADC_Average_Value(void) // 10�Σ�����ƽ��ֵ
//{
//	uint8_t adn = 0;
//	uint16_t ADC_Data_Sum = 0x0000;
//	uint16_t Array_data;
//	uint16_t Max_Buffer = 0x0000;
//	uint16_t Min_Buffer = 0xffff;
//	for (adn = 0; adn < 10; adn++)
//	{
//		Array_data = ad10_data[adn];
//		if (Max_Buffer < Array_data)
//			Max_Buffer = Array_data; // �����
//		if (Min_Buffer > Array_data)
//			Min_Buffer = Array_data; // ����С
//		ADC_Data_Sum += Array_data;
//	}
//	ADC_Data_Sum = (ADC_Data_Sum - Max_Buffer - Min_Buffer) / 8; // ȥ�����ֵ��ȥ����Сֵ����ƽ��  /8
//	return (ADC_Data_Sum);										 // ����ƽ��ֵ
//}

// uint16_t adc_value_10(uint8_t inp_n)
//{
//	uint16_t Out_AD_Buffer;
//	ADC_Getvalue(inp_n);
//	Out_AD_Buffer=ADC_Average_Value();
//	return(Out_AD_Buffer);
// }
uint16_t adc_value_10(uint8_t inp_n) // 10�Σ�����ƽ��ֵ
{
	uint16_t adn = 0;
	uint16_t sum = 0;
	uint16_t min = 0xffff;
	uint16_t max = 0;
	uint16_t av_data = 0;
	uint16_t AD_data[10]; // �����޷�����������
	ADM = ADCEn | inp_n;
	ADR = 0x60;	  // 4M   00 = Fosc/16;01 = Fosc/8;10 = Fosc/1;11 = Fosc/2?
	VREFH = 0x07; ///*IEN0 |= 0x80; //enable global interrupt*/
	IEN2 |= EADC; // �ر��ж�ad
	for (adn = 0; adn < 10; adn++)
	{
		AD_data[adn] = ADC_S(); // ����ADת������ֵ
		if (max < AD_data[adn])
			max = AD_data[adn]; // �����
		if (min > AD_data[adn])
			min = AD_data[adn]; // ����С
		sum += AD_data[adn];	// ���
	}
	ADM &= ClearEOC;
	av_data = (sum - min - max) / 8; // ȥ�����ֵ��ȥ����Сֵ����ƽ��
	return (av_data);				 // ����ƽ��ֵ
}
