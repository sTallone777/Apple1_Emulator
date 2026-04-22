#include <stdio.h>
#include <stdint.h>

// メモリ64k
uint8_t memory[65536];

// register
typedef struct CPU{
    uint16_t PC;
    uint8_t SP;
    uint8_t A;
    uint8_t X;
    uint8_t Y;
    
    uint8_t Z: 1;
    uint8_t N: 1;
} CPU;