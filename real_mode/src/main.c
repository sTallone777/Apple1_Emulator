#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <ctype.h>

#include "cpu6502.h"
#include "apple1.h"

// 终端设置（用于非阻塞获取键盘输入，按键无需按回车）
struct termios orig_termios;
void reset_terminal_mode() { tcsetattr(0, TCSANOW, &orig_termios); }
void set_conio_terminal_mode() {
    struct termios new_termios;
    tcgetattr(0, &orig_termios);
    atexit(reset_terminal_mode);
    tcgetattr(0, &new_termios);
    new_termios.c_lflag &= ~(ICANON | ECHO); // 关闭行缓冲和回显
    tcsetattr(0, TCSANOW, &new_termios);
    // 设置 stdin 为非阻塞
    int flags = fcntl(0, F_GETFL, 0);
    fcntl(0, F_SETFL, flags | O_NONBLOCK);
}

int main() {
    // 1. 设置终端非阻塞输入
    set_conio_terminal_mode();

    // 2. 实例化系统与 CPU
    Apple1 my_apple;
    init_apple1(&my_apple);

    CPU6502 cpu;
    cpu.system_context = &my_apple;
    cpu.read_bus = apple1_read;
    cpu.write_bus = apple1_write;
    cpu.enable_bcd = 1; 
    
    reset_cpu(&cpu);

    printf("Apple I 模拟器已启动。按 Ctrl+C 退出。\n");

    // 3. 模拟主循环
    while (1) {
        // --- 检查真实键盘输入 ---
        char c;
        if (read(0, &c, 1) > 0) {
            // Apple I 的字母全是大写，回车键是 \r
            if (c == '\n') c = '\r';
            else c = toupper(c);
            
            my_apple.kbd_data = c;
            my_apple.kbd_ready = 1;
        }

        // --- 每次循环执行一小段 CPU 周期 ---
        // 注意：因为指令还没实现全，当前只执行 1 个周期会导致它在遇到未知指令时停止。
        // 等你补全指令后，改成 execute(&cpu, 1) 即可。
        execute(&cpu, 1);
        
        // 短暂休眠防止 CPU 占用 100%
        usleep(10); 
    }

    return 0;
}