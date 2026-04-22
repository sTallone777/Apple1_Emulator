#ifndef CPU6502_H
#define CPU6502_H

#include <stdint.h>

typedef uint8_t  Byte;
typedef uint16_t Word;

// 定义状态寄存器标志位
#define FLAG_C (1 << 0) // Carry
#define FLAG_Z (1 << 1) // Zero
#define FLAG_I (1 << 2) // Interrupt Disable
#define FLAG_D (1 << 3) // Decimal Mode
#define FLAG_B (1 << 4) // Break
#define FLAG_U (1 << 5) // Unused (始终为1)
#define FLAG_V (1 << 6) // Overflow
#define FLAG_N (1 << 7) // Negative

// 函数指针，用于解耦内存总线
typedef Byte (*ReadFunc)(void* system_context, Word address);
typedef void (*WriteFunc)(void* system_context, Word address, Byte value);

// 6502 CPU 结构体
typedef struct {
    Word PC;      // 程序计数器
    Byte SP;      // 栈指针
    Byte A;       // 累加器
    Byte X;       // 变址寄存器X
    Byte Y;       // 变址寄存器Y
    Byte PS;      // 状态寄存器

    int enable_bcd; // 1: 支持十进制运算(Apple I), 0: 不支持(NES)
    
    // 总线接口
    void* system_context; 
    ReadFunc read_bus;
    WriteFunc write_bus;
} CPU6502;

void reset_cpu(CPU6502* cpu);
void execute(CPU6502* cpu, uint32_t cycles);

#endif // CPU6502_H