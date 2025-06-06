#include "oled_App.h"




int oled_printf(uint8_t x, uint8_t y, const char *format, ...)
{
  char buffer[512]; // 临时存储格式化后的字符串
  va_list arg;      // 处理可变参数
  int len;          // 最终字符串长度

  va_start(arg, format);
  // 安全地格式化字符串到 buffer
  len = vsnprintf(buffer, sizeof(buffer), format, arg);
  va_end(arg);

  OLED_ShowString(x, y, (uint8_t *)buffer, 16); // 改为16，支持的字体大小
  return len;
}







//// 原始的oled_proc函数
//void oled_proc(void)
//{
//  static uint32_t counter = 0;
//  static uint8_t init_flag = 0;

//  // 只在第一次调用时初始化
//  if(init_flag == 0)
//  {
//    init_flag = 1;
//    // 确保OLED已经初始化
//    OLED_Clear();
//    delay_1ms(100);
//  }

//  // 清除屏幕（注意：OLED_Clear内部会调用OLED_Refresh）
//  OLED_Clear();

//  // 显示固定文本
//  oled_printf(0, 0, "hello CIMC!");

//  // 显示计数器，验证刷新是否正常
//  oled_printf(0, 16, "Count:%d", counter++);

//  // 关键：刷新显示到OLED屏幕
//  OLED_Refresh();

//  // 添加延时，避免刷新过快
//  delay_1ms(500);
//}
