#include <stdio.h>
#include "mos6502.h"

#define LOAD_ADDR 0x0800

int main() {
    // 1. 初始化 CPU
    CPU cpu = {0}; 
    cpu.PC = LOAD_ADDR; 

    // 2. 编写并“烧录”机器码
    memory[0x0800] = INS_LDA_IM; 
    memory[0x0801] = 0x05;
    memory[0x0802] = INS_ADC_IM; 
    memory[0x0803] = 0x03;
    memory[0x0804] = INS_STA_ZP; 
    memory[0x0805] = 0x00;

    // 3. 运行 CPU 
    printf("--- 系统启动 ---\n");
    execute(&cpu, 6);

    // 4. 检查执行结果
    printf("执行完毕！\n");
    printf("当前 A 寄存器的值: %d (十六进制 0x%02X)\n", cpu.A, cpu.A);
    printf("内存地址 0x0000 的值: %d (十六进制 0x%02X)\n", memory[0x00], memory[0x00]);

    return 0;
}

/*
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "用法: %s <程序文件.bin>\n", argv[0]);
        return 1;
    }

    // 1. 初始化 CPU
    CPU cpu = {0};
    cpu.PC = LOAD_ADDR;

    // 2. 从文件加载机器码到内存
    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        fprintf(stderr, "无法打开文件: %s\n", argv[1]);
        return 1;
    }
    size_t bytes_read = fread(&memory[LOAD_ADDR], 1, sizeof(memory) - LOAD_ADDR, f);
    fclose(f);
    printf("已加载 %zu 字节到地址 0x%04X\n", bytes_read, LOAD_ADDR);

    // 3. 运行 CPU
    printf("--- 系统启动 ---\n");
    execute(&cpu, 6);

    // 4. 检查执行结果
    printf("执行完毕！\n");
    printf("当前 A 寄存器的值: %d (十六进制 0x%02X)\n", cpu.A, cpu.A);
    printf("内存地址 0x0000 的值: %d (十六进制 0x%02X)\n", memory[0x00], memory[0x00]);

    return 0;
}
*/