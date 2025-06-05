#include "adc.h"



void ADC_Init(void)
{
    adc_deinit();    // ¸´λADC
	
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);    	// ʹĜÁ¬иת»»ģʽ
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);   			// ʽ¾ݓҶԆ렍
    adc_channel_length_config(ADC0, ADC_ROUTINE_CHANNEL, 1);  			// ͨµÀŤփ£¬¹攲ש

    adc_routine_channel_config(ADC0, 0, ADC_CHANNEL_10, ADC_SAMPLETIME_56);   // ¶Թ攲ש½øАŤփ

    adc_external_trigger_source_config(ADC0, ADC_ROUTINE_CHANNEL, ADC_EXTTRIG_INSERTED_T0_CH3);   // ADC ´¥·¢Ťփ£¬ÀûӃ¶¨ʱƷ´¥·¢
    adc_external_trigger_config(ADC0, ADC_ROUTINE_CHANNEL, ENABLE);   							  // ƴӃ´¥·¢
	
    adc_enable(ADC0);   		// ʹĜADC½ӿڍ
	
    delay_1ms(1);  				// µȴý1ms

    adc_calibration_enable(ADC0);    // ADCУ׼º͸´λADCУ׼
}

void ADC_port_init(void)
{
		rcu_periph_clock_enable(RCU_GPIOC);   // GPIOCʱ֓ʹĜ
		rcu_periph_clock_enable(RCU_ADC0);    // ʹĜADCʱ֓
		
		gpio_mode_set(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_0);   // ŤփPC0ΪģĢʤȫ
		
		adc_clock_config(ADC_ADCCK_PCLK2_DIV8);   // adcʱ֓Ťփ
		
		ADC_Init();  
		
		adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL); //  ¹攲²ɑùȭ¼þ´¥·¢
		

}
