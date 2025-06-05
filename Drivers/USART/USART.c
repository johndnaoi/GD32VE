#include "USART.h"

#define RX_BUFFER_SIZE      256      // ���ջ�������С
#define RX_TIMEOUT          (20)     // ���ճ�ʱ��ֵ����λ����оƬ��ͨ��Ϊbitʱ�䣩

extern uint8_t rx_buffer[RX_BUFFER_SIZE];  // ���ڽ��ջ�����
extern uint16_t rx_index;                  // ��ǰ������������
extern uint8_t frame_ready;                // һ֡���ݽ�����ɱ�־


int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t)ch);  // ����һ���ֽڵ�USART0
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE)); // �ȴ����ͻ�����Ϊ��
    return ch;
}


void USART0_Config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);      // ʹ��GPIOAʱ��
    rcu_periph_clock_enable(RCU_USART0);     // ʹ��USART0����ʱ��

    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_9 | GPIO_PIN_10);  // ����PA9/PA10ΪUSART0����
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_9 | GPIO_PIN_10); 
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9 | GPIO_PIN_10);

    usart_deinit(USART0);                                // ����USART0
    usart_word_length_set(USART0, USART_WL_8BIT);         // 8λ����λ
    usart_stop_bit_set(USART0, USART_STB_1BIT);           // 1λֹͣλ
    usart_parity_config(USART0, USART_PM_NONE);           // ����żУ��
    usart_baudrate_set(USART0, 115200U);                  // ���ò�����Ϊ115200
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);   // ʹ�ܽ���
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE); // ʹ�ܷ���

    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE); // ����RTS
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE); // ����CTS

    // 接收超时中断
    usart_receiver_timeout_threshold_config(USART0, RX_TIMEOUT);
    usart_receiver_timeout_enable(USART0);

    // ʹ�ܽ��ճ�ʱ�жϣ�֡������
    usart_interrupt_enable(USART0, USART_INT_RT);

    usart_enable(USART0);  // ����USART0
}




void USART0_SendData(uint16_t *buf, uint16_t len)
{
    uint16_t t;
    for (t = 0; t < len; t++) {
        while (usart_flag_get(USART0, USART_FLAG_TC) == RESET); // �ȴ��������
        usart_data_transmit(USART0, buf[t]);                     // ����һ����
    }
    while (usart_flag_get(USART0, USART_FLAG_TC) == RESET);      // ���һ���ֽ�Ҳ���
}


void USART0_IRQHandler(void)
{
	

    if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)) 
    {
        uint8_t data = usart_data_receive(USART0);
        
				
     
        if(rx_index < RX_BUFFER_SIZE) {
            rx_buffer[rx_index++] = data;
        }
        
        usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);
    }
    
 
    if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_RT))
    {
        
        frame_ready = 1;
        
     
        usart_flag_clear(USART0, USART_FLAG_RT);
        usart_data_receive(USART0); 
        
        usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RT);
    }
}



