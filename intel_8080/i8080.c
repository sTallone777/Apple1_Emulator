#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// --- 1. 结构体定义 ---
typedef struct {
    uint8_t a, b, c, d, e, h, l;
    uint8_t f;
    uint16_t sp;
    uint16_t pc;
    uint8_t *memory;
    uint8_t halted;
} Intel8080;

// --- 2. 初始化与辅助函数 ---
Intel8080* init_8080() {
    Intel8080* cpu = calloc(1, sizeof(Intel8080));
    cpu->memory = calloc(0x10000, 1); // 64KB 内存
    return cpu;
}

// 打印 CPU 状态，方便你研究逻辑
void debug_print(Intel8080* cpu) {
    printf("PC: %04X | OP: %02X | A:%02X B:%02X C:%02X D:%02X E:%02X H:%02X L:%02X | SP:%04X\n", 
           cpu->pc, cpu->memory[cpu->pc], cpu->a, cpu->b, cpu->c, cpu->d, cpu->e, cpu->h, cpu->l, cpu->sp);
}

// 获取寄存器指针的辅助函数 (根据图中 DDD/SSS 编码)
uint8_t* get_reg_ptr(Intel8080* cpu, uint8_t code) {
    switch (code) {
        case 0: return &cpu->b; case 1: return &cpu->c;
        case 2: return &cpu->d; case 3: return &cpu->e;
        case 4: return &cpu->h; case 5: return &cpu->l;
        case 6: { // 内存 M ([HL])
            uint16_t addr = (cpu->h << 8) | cpu->l;
            return &cpu->memory[addr];
        }
        case 7: return &cpu->a;
    }
    return NULL;
}

// --- 3. 核心执行引擎 ---
void emulate_8080_op(Intel8080* cpu) {
    uint8_t opcode = cpu->memory[cpu->pc];
    
    // 调试打印：让你看到每一步发生了什么
    debug_print(cpu);

    // 译码与执行
    // 这里只实现了 MOV, MVI, INR, HLT 等基础指令作为演示
    if ((opcode & 0xC0) == 0x40) { // MOV ddd, sss (01 DDD SSS)
        if (opcode == 0x76) { // 特例：HLT
            cpu->halted = 1;
        } else {
            uint8_t ddd = (opcode >> 3) & 0x07;
            uint8_t sss = (opcode & 0x07);
            *get_reg_ptr(cpu, ddd) = *get_reg_ptr(cpu, sss);
            cpu->pc += 1;
        }
    } 
    else if ((opcode & 0xC7) == 0x06) { // MVI ddd, data (00 DDD 110)
        uint8_t ddd = (opcode >> 3) & 0x07;
        *get_reg_ptr(cpu, ddd) = cpu->memory[cpu->pc + 1];
        cpu->pc += 2;
    }
    else if ((opcode & 0xC7) == 0x04) { // INR ddd (00 DDD 100)
        uint8_t ddd = (opcode >> 3) & 0x07;
        (*get_reg_ptr(cpu, ddd))++;
        // 注意：真实实现需在这里更新 Flags，此处为演示简化了
        cpu->pc += 1;
    }
    else {
        printf("未实现的指令: %02X\n", opcode);
        cpu->halted = 1;
    }
}

// --- 4. 主函数 ---
int main() {
    Intel8080* cpu = init_8080();

    // 手动在内存中写一段简单的“程序”测试逻辑：
    // 1. MVI A, 0x10 (3E 10) -> 把 0x10 送入 A
    // 2. INR A       (3C)    -> A 自增 1 (变成 0x11)
    // 3. MOV B, A    (47)    -> 把 A 的值给 B (B 变成 0x11)
    // 4. HLT         (76)    -> 停机
    uint8_t test_program[] = {0x3E, 0x10, 0x3C, 0x47, 0x76};
    memcpy(cpu->memory, test_program, sizeof(test_program));

    printf("开始模拟器运行...\n");
    while (!cpu->halted) {
        emulate_8080_op(cpu);
    }
    printf("模拟结束。\n");

    free(cpu->memory);
    free(cpu);
    return 0;
}