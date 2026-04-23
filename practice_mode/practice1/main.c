#include <stdio.h>
#include "mos6502.h" // 引入 CPU 的说明书

int main() {
    // 1. 初始化 CPU
    CPU cpu = {0}; 
    cpu.PC = 0x0800; 

    // 2. 编写并“烧录”机器码
    memory[0x0800] = INS_LDA_IM; memory[0x0801] = 0x05;
    memory[0x0802] = INS_ADC_IM; memory[0x0803] = 0x03;
    memory[0x0804] = INS_STA_ZP; memory[0x0805] = 0x00;

    // 3. 运行 CPU 
    printf("--- 系统启动 ---\n");
    execute(&cpu, 6);

    // 4. 检查执行结果
    printf("执行完毕！\n");
    printf("当前 A 寄存器的值: %d (十六进制 0x%02X)\n", cpu.A, cpu.A);
    printf("内存地址 0x0000 的值: %d (十六进制 0x%02X)\n", memory[0x00], memory[0x00]);

    return 0;
}