#include "scheduler.h"


uint8_t task_num;

typedef struct {
    void (*task_func)(void);
    uint32_t rate_ms;
    uint32_t last_run;
} task_t;




static task_t scheduler_task[] =
{
	{led_proc,1,0},
	{adc_proc,100,0},
  {oled_proc,10,0}
};


void scheduler_init(void)
{
   task_num = sizeof(scheduler_task) / sizeof(task_t);
}

void scheduler_run(void)
{
    uint32_t now_time = get_system_ms();  // 移到循环外获取当前时间
    
    for (uint8_t i = 0; i < task_num; i++)
    {
        // 使用时间差值比较，避免溢出问题
        if ((int32_t)(now_time - scheduler_task[i].last_run) >= (int32_t)scheduler_task[i].rate_ms)
        {
            scheduler_task[i].last_run = now_time;
            scheduler_task[i].task_func();
        }
    }
}



