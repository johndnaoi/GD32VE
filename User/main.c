#include "HeaderFiles.h"




int main(void)
{
   	
	systick_config();   
	init_cycle_counter(false);	
	delay_ms(200);
	
	
	
	LED_Init();
	USART0_Config(); 
	OLED_Init();
	ADC_port_init();
	
	
//	nvic_config();		
//	facard_proc();
	scheduler_init();
	
	
	while(1)
	{
		scheduler_run();
		// 测试printf函数
		//oled_proc();
		//adc_proc();
	}
	
}


