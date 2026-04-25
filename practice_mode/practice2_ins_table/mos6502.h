#ifndef CPU6502_H
#define CPU6502_H

#include <stdint.h>

// メモリ64k
extern uint8_t memory[65536];

// register
typedef struct {
    uint16_t PC;
    uint8_t SP;
    uint8_t A;
    uint8_t X;
    uint8_t Y;
    
    uint8_t Z: 1;
    uint8_t N: 1;
} CPU;

typedef void (*InstructionFunc)(CPU *cpu, int *cycles, uint8_t value);
typedef uint8_t (*FindAddressFunc)(uint16_t address);

typedef struct {
    char *name;          // 指令名称
    InstructionFunc func;  // 执行指令逻辑的函数指针
    FindAddressFunc addr;  // 寻址模式逻辑的函数指针
    uint8_t cycles;      // 基础时钟周期
} Instruction;

#define INS_LDA_IM  0xA9
#define INS_ADC_IM  0x69
#define INS_STA_ZP  0x85

uint8_t read_byte(uint16_t address);
void execute(CPU *cpu, int cycles);

#endif