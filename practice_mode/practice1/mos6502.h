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

#define INS_LDA_IM  0xA9
#define INS_ADC_IM  0x69
#define INS_STA_ZP  0x85

uint8_t read_byte(uint16_t address);
void execute(CPU *cpu, int cycles);

#endif