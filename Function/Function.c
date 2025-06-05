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
    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);	// 设置中断优先级分组
    nvic_irq_enable(SDIO_IRQn, 0, 0);					// 使能SDIO中断，优先级为0
}


void write_file(void)
{
    printf("Input data (press Enter to save):\r\n");
    
    uint16_t index = 0;    
    while(1){
        if(usart_flag_get(USART0, USART_FLAG_RBNE) != RESET){
            char ch = usart_data_receive(USART0); 						// 如果接收缓冲区非空，从USART0接收一个字符
            if(ch == '\r'){												// 检查接收到的字符是否为回车键（'\r'）
                filebuffer[index] = '\0';  								// 如果是回车键，在当前位置添加字符串结束符 '\0'
                break;													// 跳出循环，结束数据接收
            }
            filebuffer[index++] = ch;        							// 存储接收到的字符    
            if(index >= sizeof(filebuffer)-1) break;					// 如果缓冲区满则结束接收
        }
    }
}

void facard_proc(void)
{
	uint16_t k = 5;
	DSTATUS stat = 0;
	
	do
	{
		stat = disk_initialize(0); 			//初始化SD卡（设备号0）,物理驱动器编号,每个物理驱动器（如硬盘、U 盘等）通常都被分配一个唯一的编号。
	}while((stat != 0) && (--k));			//如果初始化失败，重试最多k次。
    
    printf("SD Card disk_initialize:%d\r\n",stat);
    f_mount(0, &fs);						 //挂载SD卡的文件系统（设备号0）。
    printf("SD Card f_mount:%d\r\n",stat);

	if(RES_OK == stat)						 //返回挂载结果（FR_OK 表示成功）。
	{        
        printf("\r\nSD Card Initialize Success!\r\n");
	 
        result = f_open(&fdst, "0:/CIMC1.TXT", FA_CREATE_ALWAYS | FA_WRITE);		//在SD卡上创建文件FATFS.TXT。
	 
		write_file();	//调用写文件

		//result = f_write(&fdst, textfilebuffer, sizeof(textfilebuffer), &bw); 	//将textfilebuffer中的数据写入文件。
		result = f_write(&fdst, filebuffer, sizeof(filebuffer), &bw);				//将filebuffer中的数据写入文件。
        
		/**********检查写入结果 begin****************/
		if(FR_OK == result)		
                printf("FATFS FILE write Success!\r\n");
        else
		{
                printf("FATFS FILE write failed!\r\n");
        }
		/**********检查写入结果 end****************/
		
        f_close(&fdst);//关闭文件
		
		
        f_open(&fdst, "0:/CIMC1.TXT", FA_OPEN_EXISTING | FA_READ);	//以只读方式重新打开文件
        br = 1;
		
		/**********循环读取文件内容 begin****************/
        for(;;)
			{
			// 清空缓冲区
            for (count=0; count<128; count++)
			{
				buffer[count]=0;
			}
			// 读取文件内容到buffer
            result = f_read(&fdst, buffer, sizeof(buffer), &br);
            if ((0 == result)|| (0 == br))
			{
                break;
			}
        }
		/**********循环读取文件内容 end****************/
		
		// 比较读取的内容与写入的内容是否一致
        if(SUCCESS == memory_compare(buffer, filebuffer, 128))
		{
			printf("FATFS Read File Success!\r\nThe content is:%s\r\n",buffer);
		}
        else
		{
            printf("FATFS FILE read failed!\n");            
        }
         f_close(&fdst);//关闭文件
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

