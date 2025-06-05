#include "LED.h"


void LED_Init(void)
{
    // 开启 GPIOE 和 GPIOC 的外设时钟，供LED使用
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_GPIOC);

    // 配置 GPIOE的3~6号引脚为推挽输出，无上下拉
    gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, 
                  GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, 
                            GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);
    gpio_bit_reset(GPIOE, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6); // 初始输出低电平（LED熄灭）

    // 配置 GPIOC的1号和13号引脚为输出（共6个LED控制引脚）
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, 
                  GPIO_PIN_1 | GPIO_PIN_13);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, 
                            GPIO_PIN_1 | GPIO_PIN_13);
    gpio_bit_reset(GPIOC, GPIO_PIN_1 | GPIO_PIN_13); // 初始输出低电平
}


void led_disp(uint8_t *ucLed)
{
    uint8_t temp = 0x00;               // 用于存放LED的6位状态
    static uint8_t temp_old = 0xFF;    // 上次显示的状态（初始值不同于任何实际值）

    // 遍历6个LED，组合成一个6位的二进制状态位
    for (int i = 0; i < 6; i++) {
        temp |= (ucLed[i] << i);       // ucLed[i]非0表示亮，左移后合成bit位
    }

    // 如果当前状态与上次不同，才进行GPIO更新（节省资源）
    if (temp != temp_old) {
        gpio_bit_write(GPIOC, GPIO_PIN_1,  (temp & 0x01) ? SET : RESET);  // LED1
        gpio_bit_write(GPIOE, GPIO_PIN_3,  (temp & 0x02) ? SET : RESET);  // LED2
        gpio_bit_write(GPIOE, GPIO_PIN_4,  (temp & 0x04) ? SET : RESET);  // LED3
        gpio_bit_write(GPIOE, GPIO_PIN_5,  (temp & 0x08) ? SET : RESET);  // LED4
        gpio_bit_write(GPIOE, GPIO_PIN_6,  (temp & 0x10) ? SET : RESET);  // LED5
        gpio_bit_write(GPIOC, GPIO_PIN_13, (temp & 0x20) ? SET : RESET);  // LED6
        temp_old = temp; // 更新上次显示的状态
    }
}

