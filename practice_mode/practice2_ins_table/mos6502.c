#include <stdio.h>
#include "mos6502.h"

uint8_t memory[65536];

uint8_t read_byte(uint16_t address) {
    return memory[address];
}

void ins_lda(CPU *cpu, int *cycles, uint8_t value) {
    cpu->A = value;
    cpu->Z = (cpu->A == 0);
    (*cycles)--;
}

void ins_adc(CPU *cpu, int *cycles, uint8_t value) {
    cpu->A += value;
    cpu->Z = (cpu->A == 0);
    (*cycles)--;
}

void ins_sta(CPU *cpu, int *cycles, uint8_t target_addr) {
    memory[target_addr] = cpu->A;
    (*cycles)--;
}

Instruction lookup[256] = {
    [INS_LDA_IM] = {"LDA", ins_lda, read_byte, 2},
    [INS_ADC_IM] = {"ADC", ins_adc, read_byte, 2},
    [INS_STA_ZP] = {"STA", ins_sta, read_byte, 2},
};

void execute(CPU *cpu, int cycles) {
    while (cycles > 0) {
        uint8_t opcode = read_byte(cpu->PC);
        cpu->PC++;
        cycles--;
        
        uint8_t value = lookup[opcode].addr(cpu->PC);
        cpu->PC++;
        lookup[opcode].func(cpu, &cycles, value);
    }
}

/*
void execute(CPU *cpu, int cycles) {
    while (cycles > 0) {
        uint8_t opcode = read_byte(cpu->PC);
        cpu->PC++;
        cycles--;

        switch (opcode) {
            case INS_LDA_IM: {
                uint8_t value = read_byte(cpu->PC);
                cpu->PC++;
                cpu->A = value;
                cpu->Z = (cpu->A == 0);
                cycles--;
            } break;

            case INS_ADC_IM: {
                uint8_t value = read_byte(cpu->PC);
                cpu->PC++;
                cpu->A += value;                    
                cpu->Z = (cpu->A == 0);
                cycles--;
            } break;

            case INS_STA_ZP: {
                uint8_t target_addr = read_byte(cpu->PC); 
                cpu->PC++;
                memory[target_addr] = cpu->A;       
                cycles--;
            } break;

            default:
                printf("致命错误: 遇到了未知的指令 0x%02X\n", opcode);
                return;
        }
    }
}
*/