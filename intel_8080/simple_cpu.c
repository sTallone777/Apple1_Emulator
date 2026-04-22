#include <stdio.h>

typedef struct {
    unsigned char A;    // 寄存器 A
    unsigned short PC;  // 程序计数器
} CPU;

void run_cpu(CPU *cpu, unsigned char *mem) {
    int running = 1;
    while (running) {
        unsigned char opcode = mem[cpu->PC]; // 取指
        
        switch (opcode) {
            case 0x01: // LDA: 把下一个字节读入寄存器 A
                cpu->A = mem[++cpu->PC];
                printf("LDA: Register A = %d\n", cpu->A);
                break;
                
            case 0x02: // INC: 寄存器 A 自增
                cpu->A++;
                printf("INC: Register A = %d\n", cpu->A);
                break;

            case 0xFF: // 停机指令
                running = 0;
                break;
        }
        cpu->PC++; // 移动到下一条指令
    }
}

int main() {
    CPU my_cpu = {0, 0};
    unsigned char program[] = {0x01, 10, 0x02, 0x02, 0xFF}; // 10进A，加两次1
    run_cpu(&my_cpu, program);
    return 0;
}