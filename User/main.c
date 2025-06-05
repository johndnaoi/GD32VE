#include "HeaderFiles.h"




int main(void)
{
	
	systick_config();    
	LED_Init();
	USART0_Config(); 
	OLED_Init();

	
	
//	nvic_config();		
//	facard_proc();
	
	// 先测试基本OLED功能
	oled_test_basic();
	delay_1ms(2000);

	while(1)
	{
		oled_proc();
	}
	
}


