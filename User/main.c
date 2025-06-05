#include "HeaderFiles.h"




int main(void)
{
	
	systick_config();    
	LED_Init();
	USART0_Config(); 
	OLED_Init();
	ADC_port_init();
	
	
//	nvic_config();		
//	facard_proc();
	






	

	while(1)
	{
		// 测试printf函数
		//oled_proc();
		//adc_proc();
	}
	
}


