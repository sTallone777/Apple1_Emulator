#ifndef APPLE1_H
#define APPLE1_H

#include "cpu6502.h"

// Apple I 系统结构
typedef struct {
    Byte ram[8192];  // 8KB 主存
    Byte rom[256];   // 256 字节 Woz Monitor
    
    // PIA (外部接口适配器) 状态
    Byte kbd_data;   // 键盘输入数据
    int  kbd_ready;  // 1: 有键按下，0: 空闲
} Apple1;

void init_apple1(Apple1* sys);
Byte apple1_read(void* context, Word address);
void apple1_write(void* context, Word address, Byte value);

#endif // APPLE1_H