#include "USART.h"

#define RX_BUFFER_SIZE      256      // 接收缓冲区大小
#define RX_TIMEOUT          (20)     // 接收超时阈值（单位依赖芯片，通常为bit时间）

extern uint8_t rx_buffer[RX_BUFFER_SIZE];  // 串口接收缓冲区
extern uint16_t rx_index;                  // 当前接收数据索引
extern uint8_t frame_ready;                // 一帧数据接收完成标志


int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t)ch);  // 发送一个字节到USART0
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE)); // 等待发送缓冲区为空
    return ch;
}


void USART0_Config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);      // 使能GPIOA时钟
    rcu_periph_clock_enable(RCU_USART0);     // 使能USART0外设时钟

    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_9 | GPIO_PIN_10);  // 设置PA9/PA10为USART0功能
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_9 | GPIO_PIN_10); 
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9 | GPIO_PIN_10);

    usart_deinit(USART0);                                // 重置USART0
    usart_word_length_set(USART0, USART_WL_8BIT);         // 8位数据位
    usart_stop_bit_set(USART0, USART_STB_1BIT);           // 1位停止位
    usart_parity_config(USART0, USART_PM_NONE);           // 无奇偶校验
    usart_baudrate_set(USART0, 115200U);                  // 设置波特率为115200
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);   // 使能接收
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE); // 使能发送

    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE); // 禁用RTS
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE); // 禁用CTS

    // 鎺ユ敹瓒呮椂涓柇
    usart_receiver_timeout_threshold_config(USART0, RX_TIMEOUT);
    usart_receiver_timeout_enable(USART0);

    // 使能接收超时中断（帧结束）
    usart_interrupt_enable(USART0, USART_INT_RT);

    usart_enable(USART0);  // 启用USART0
}




void USART0_SendData(uint16_t *buf, uint16_t len)
{
    uint16_t t;
    for (t = 0; t < len; t++) {
        while (usart_flag_get(USART0, USART_FLAG_TC) == RESET); // 等待发送完成
        usart_data_transmit(USART0, buf[t]);                     // 发送一个字
    }
    while (usart_flag_get(USART0, USART_FLAG_TC) == RESET);      // 最后一个字节也完成
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



