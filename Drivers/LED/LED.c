#include "LED.h"


void LED_Init(void)
{
    // ���� GPIOE �� GPIOC ������ʱ�ӣ���LEDʹ��
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_GPIOC);

    // ���� GPIOE��3~6������Ϊ�����������������
    gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, 
                  GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, 
                            GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);
    gpio_bit_reset(GPIOE, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6); // ��ʼ����͵�ƽ��LEDϨ��

    // ���� GPIOC��1�ź�13������Ϊ�������6��LED�������ţ�
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, 
                  GPIO_PIN_1 | GPIO_PIN_13);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, 
                            GPIO_PIN_1 | GPIO_PIN_13);
    gpio_bit_reset(GPIOC, GPIO_PIN_1 | GPIO_PIN_13); // ��ʼ����͵�ƽ
}


void led_disp(uint8_t *ucLed)
{
    uint8_t temp = 0x00;               // ���ڴ��LED��6λ״̬
    static uint8_t temp_old = 0xFF;    // �ϴ���ʾ��״̬����ʼֵ��ͬ���κ�ʵ��ֵ��

    // ����6��LED����ϳ�һ��6λ�Ķ�����״̬λ
    for (int i = 0; i < 6; i++) {
        temp |= (ucLed[i] << i);       // ucLed[i]��0��ʾ�������ƺ�ϳ�bitλ
    }

    // �����ǰ״̬���ϴβ�ͬ���Ž���GPIO���£���ʡ��Դ��
    if (temp != temp_old) {
        gpio_bit_write(GPIOC, GPIO_PIN_1,  (temp & 0x01) ? SET : RESET);  // LED1
        gpio_bit_write(GPIOE, GPIO_PIN_3,  (temp & 0x02) ? SET : RESET);  // LED2
        gpio_bit_write(GPIOE, GPIO_PIN_4,  (temp & 0x04) ? SET : RESET);  // LED3
        gpio_bit_write(GPIOE, GPIO_PIN_5,  (temp & 0x08) ? SET : RESET);  // LED4
        gpio_bit_write(GPIOE, GPIO_PIN_6,  (temp & 0x10) ? SET : RESET);  // LED5
        gpio_bit_write(GPIOC, GPIO_PIN_13, (temp & 0x20) ? SET : RESET);  // LED6
        temp_old = temp; // �����ϴ���ʾ��״̬
    }
}

