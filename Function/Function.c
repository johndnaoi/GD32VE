#include "Function.h"


#define  SFLASH_ID                     0xC84013
#define BUFFER_SIZE                    256
#define TX_BUFFER_SIZE                 BUFFER_SIZE
#define RX_BUFFER_SIZE                 BUFFER_SIZE
#define  FLASH_WRITE_ADDRESS           0x000000
#define  FLASH_READ_ADDRESS            FLASH_WRITE_ADDRESS

/*				 sd				*/
uint32_t flash_id = 0;
uint8_t  tx_buffer[TX_BUFFER_SIZE];
uint8_t  rx_buffer[TX_BUFFER_SIZE];
uint16_t i = 0, count, result = 0;
uint8_t  is_successful = 0;

/*				flash			*/
FIL fdst;
FATFS fs;
UINT br, bw;
//BYTE textfilebuffer[2048] = "GD32MCU FATFS TEST!\r\n";
BYTE buffer[128];
BYTE filebuffer[128];

ErrStatus memory_compare(uint8_t* src,uint8_t* dst,uint16_t length);

/*				usart				*/
uint8_t rx_buffer[RX_BUFFER_SIZE];  
uint16_t rx_index = 0;              
uint8_t frame_ready = 0;           

/*					adc				*/
int adc_value;    
float Vol_Value;







void nvic_config(void);
void write_file(void);
void facard_proc(void);


void adc_proc(void)
{
	adc_flag_clear(ADC0,ADC_FLAG_EOC); 
	while(SET != adc_flag_get(ADC0,ADC_FLAG_EOC)){}  
        
        adc_value = ADC_RDATA(ADC0);    				
		Vol_Value = adc_value*3.3/4095;  				
				
		printf("num=%d	project=%.2f V\r\n",adc_value, Vol_Value);  
				
        delay_1ms(1000);  
}



void nvic_config(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);	// �����ж����ȼ�����
    nvic_irq_enable(SDIO_IRQn, 0, 0);					// ʹ��SDIO�жϣ����ȼ�Ϊ0
}


void write_file(void)
{
    printf("Input data (press Enter to save):\r\n");
    
    uint16_t index = 0;    
    while(1){
        if(usart_flag_get(USART0, USART_FLAG_RBNE) != RESET){
            char ch = usart_data_receive(USART0); 						// ������ջ������ǿգ���USART0����һ���ַ�
            if(ch == '\r'){												// �����յ����ַ��Ƿ�Ϊ�س�����'\r'��
                filebuffer[index] = '\0';  								// ����ǻس������ڵ�ǰλ������ַ��������� '\0'
                break;													// ����ѭ�����������ݽ���
            }
            filebuffer[index++] = ch;        							// �洢���յ����ַ�    
            if(index >= sizeof(filebuffer)-1) break;					// ��������������������
        }
    }
}

void facard_proc(void)
{
	uint16_t k = 5;
	DSTATUS stat = 0;
	
	do
	{
		stat = disk_initialize(0); 			//��ʼ��SD�����豸��0��,�������������,ÿ����������������Ӳ�̡�U �̵ȣ�ͨ����������һ��Ψһ�ı�š�
	}while((stat != 0) && (--k));			//�����ʼ��ʧ�ܣ��������k�Ρ�
    
    printf("SD Card disk_initialize:%d\r\n",stat);
    f_mount(0, &fs);						 //����SD�����ļ�ϵͳ���豸��0����
    printf("SD Card f_mount:%d\r\n",stat);

	if(RES_OK == stat)						 //���ع��ؽ����FR_OK ��ʾ�ɹ�����
	{        
        printf("\r\nSD Card Initialize Success!\r\n");
	 
        result = f_open(&fdst, "0:/CIMC1.TXT", FA_CREATE_ALWAYS | FA_WRITE);		//��SD���ϴ����ļ�FATFS.TXT��
	 
		write_file();	//����д�ļ�

		//result = f_write(&fdst, textfilebuffer, sizeof(textfilebuffer), &bw); 	//��textfilebuffer�е�����д���ļ���
		result = f_write(&fdst, filebuffer, sizeof(filebuffer), &bw);				//��filebuffer�е�����д���ļ���
        
		/**********���д���� begin****************/
		if(FR_OK == result)		
                printf("FATFS FILE write Success!\r\n");
        else
		{
                printf("FATFS FILE write failed!\r\n");
        }
		/**********���д���� end****************/
		
        f_close(&fdst);//�ر��ļ�
		
		
        f_open(&fdst, "0:/CIMC1.TXT", FA_OPEN_EXISTING | FA_READ);	//��ֻ����ʽ���´��ļ�
        br = 1;
		
		/**********ѭ����ȡ�ļ����� begin****************/
        for(;;)
			{
			// ��ջ�����
            for (count=0; count<128; count++)
			{
				buffer[count]=0;
			}
			// ��ȡ�ļ����ݵ�buffer
            result = f_read(&fdst, buffer, sizeof(buffer), &br);
            if ((0 == result)|| (0 == br))
			{
                break;
			}
        }
		/**********ѭ����ȡ�ļ����� end****************/
		
		// �Ƚ϶�ȡ��������д��������Ƿ�һ��
        if(SUCCESS == memory_compare(buffer, filebuffer, 128))
		{
			printf("FATFS Read File Success!\r\nThe content is:%s\r\n",buffer);
		}
        else
		{
            printf("FATFS FILE read failed!\n");            
        }
         f_close(&fdst);//�ر��ļ�
	} 
}

/*!
    \brief      memory compare function
    \param[in]  src: source data pointer
    \param[in]  dst: destination data pointer
    \param[in]  length: the compare data length
    \param[out] none
    \retval     ErrStatus: ERROR or SUCCESS
*/
ErrStatus memory_compare(uint8_t* src, uint8_t* dst, uint16_t length) 
{
    while(length --){
        if(*src++ != *dst++)
            return ERROR;
    }
    return SUCCESS;
}

/****************************End*****************************/

