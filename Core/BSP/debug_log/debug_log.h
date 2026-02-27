#ifndef __DEBUG_LOG_H
#define __DEBUG_LOG_H

// 【跨文件声明】：告诉全系统的文件，有一个牛逼的 DMA 打印函数可以使用
void DMA_Printf(const char *format, ...);

#endif /* __DEBUG_LOG_H */