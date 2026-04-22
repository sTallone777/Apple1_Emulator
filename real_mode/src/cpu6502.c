#include "cpu6502.h"
#include <stdio.h>

// 内部辅助：读写内存
static Byte read_mem(CPU6502* cpu, Word addr) {
    return cpu->read_bus(cpu->system_context, addr);
}

static void write_mem(CPU6502* cpu, Word addr, Byte val) {
    cpu->write_bus(cpu->system_context, addr, val);
}

// 内部辅助：设置/读取标志位
static void set_flag(CPU6502* cpu, Byte flag, int condition) {
    if (condition) cpu->PS |= flag;
    else           cpu->PS &= ~flag;
}

// ==========================================
// 寻址方式 (Addressing Modes) 返回目标内存地址
// ==========================================

// 立即数寻址 (Immediate): 数据紧跟在操作码后面
static Word Addr_IMM(CPU6502* cpu) {
    return cpu->PC++;
}

// 绝对寻址 (Absolute): 紧跟 16 位地址（小端序）
static Word Addr_ABS(CPU6502* cpu) {
    Word lo = read_mem(cpu, cpu->PC++);
    Word hi = read_mem(cpu, cpu->PC++);
    return (hi << 8) | lo;
}

// 零页寻址 (Zero Page): 紧跟 8 位地址，高位为 0x00
static Word Addr_ZP(CPU6502* cpu) {
    return read_mem(cpu, cpu->PC++);
}

// ==========================================
// CPU 核心逻辑
// ==========================================

void reset_cpu(CPU6502* cpu) {
    // 从 0xFFFC 读取启动向量
    Word lo = read_mem(cpu, 0xFFFC);
    Word hi = read_mem(cpu, 0xFFFD);
    cpu->PC = (hi << 8) | lo;
    
    cpu->SP = 0xFD;
    cpu->A = 0;
    cpu->X = 0;
    cpu->Y = 0;
    cpu->PS = FLAG_U | FLAG_I; // 启动时屏蔽中断，U位置1
}

void execute(CPU6502* cpu, uint32_t cycles) {
    while (cycles > 0) {
        Byte opcode = read_mem(cpu, cpu->PC++);
        Word addr; // 临时保存目标地址
        Byte val;  // 临时保存读取的值

        // 开始译码
        switch (opcode) {
            
            // --- LDA (加载到累加器) ---
            case 0xA9: // LDA Immediate
                addr = Addr_IMM(cpu);
                cpu->A = read_mem(cpu, addr);
                set_flag(cpu, FLAG_Z, cpu->A == 0);
                set_flag(cpu, FLAG_N, cpu->A & 0x80);
                cycles -= 2; break;
                
            case 0xAD: // LDA Absolute
                addr = Addr_ABS(cpu);
                cpu->A = read_mem(cpu, addr);
                set_flag(cpu, FLAG_Z, cpu->A == 0);
                set_flag(cpu, FLAG_N, cpu->A & 0x80);
                cycles -= 4; break;

            // --- STA (存储累加器到内存) ---
            case 0x8D: // STA Absolute
                addr = Addr_ABS(cpu);
                write_mem(cpu, addr, cpu->A);
                cycles -= 4; break;

            // --- JMP (无条件跳转) ---
            case 0x4C: // JMP Absolute
                addr = Addr_ABS(cpu);
                cpu->PC = addr; 
                cycles -= 3; break;

            // --- NOP (空操作) ---
            case 0xEA: // NOP
                cycles -= 2; break;

            // TODO: 请继续参考 6502 指令表，在这里添加剩下的 case
            // 例如 ADC, SBC, JSR, RTS, 分支指令 (BEQ, BNE...) 等

            default:
                // 遇到未实现或非法指令，打印警告
                // printf("未实现的指令: 0x%02X 在 PC: 0x%04X\n", opcode, cpu->PC - 1);
                cycles--; // 防止死循环
                break;
        }
    }
}