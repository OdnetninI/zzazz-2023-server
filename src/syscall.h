/***************************************************************************************
*    Copyright © 2023 OdnetninI                                                        *
*                                                                                      *
*    Permission is hereby granted, free of charge, to any person obtaining a copy of   *
*    this software and associated documentation files (the “Software”), to deal in     *
*    the Software without restriction, including without limitation the rights to      *
*    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies     *
*    of the Software, and to permit persons to whom the Software is furnished to do    *
*    so, subject to the following conditions:                                          *
*                                                                                      *
*    The above copyright notice and this permission notice shall be included in all    *
*    copies or substantial portions of the Software.                                   *
*                                                                                      *
*    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR        *
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,          *
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE       *
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, *
*    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN   *
*    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.        *
****************************************************************************************/

void read_file(uint16_t addr, uint16_t file) {
    char filename[16];
    snprintf(filename, 16, "fs/%d/%02x", server_id, file);

    FILE* f_file = fopen(filename, "r");
    if (!f_file) return;

    fseek(f_file, 0, SEEK_END);
    uint16_t f_file_size = ftell(f_file);
    rewind(f_file);
    fread(&(memory[addr]), f_file_size, 1, f_file);
    fclose(f_file);    
}

uint8_t math_test_current = 0;
#define math_test_max 8

struct _math_test {
    uint16_t question;
    uint16_t answer;
};

struct _math_test math_test[math_test_max] = {
    {0x0539, 0x0024},
    {0x4c22, 0x008b},
    {0x9e3f, 0x00c9},
    {0x9610, 0x00c4},
    {0x0000, 0x0000},
    {0x403d, 0x0080},
    {0x3021, 0x006f},
    {0x1e61, 0x0058},
};

enum SYSCALLS {
    SYS_UNKNOWN = 0x00,
    SYS_PRINT,
    SYS_READ,
    SYS_EXIT,
    SYS_READFILE,
    SYS_RAND,

    SYS_MATH_GET = 0x64,
    SYS_MATH_CHECK_AND_NEXT = 0x65,
    SYS_MATH_RESET = 0x66
};

bool syscall(uint8_t id) {
    switch (id) {
        case SYS_PRINT: printf("%c", cpu.R0 & 0xff); break;
        case SYS_READ:
            char c = 0;
            scanf("%c", &c);
            cpu.R0 = c;
            break;
        case SYS_EXIT: printf("Exit\n"); exit(0);
        case SYS_READFILE: read_file(cpu.R1, cpu.R0); break;
        case SYS_RAND: cpu.R0 = rand(); break;


        case SYS_MATH_GET: cpu.R0 = math_test[math_test_current].question; break;
        case SYS_MATH_CHECK_AND_NEXT: 
            if (cpu.R0 == math_test[math_test_current].answer) {
                if(math_test_current + 1 == math_test_max) {
                    printf("Test successful! FOOLS2023_{UnknownArchitectureMath}\n");
                    cpu.R0 = 0x00;
                }
                else {
                    math_test_current++;
                    cpu.R0 = 0x01;
                }
            }
            else {
                cpu.R0 = 0x02;
            }
            break;
        case SYS_MATH_RESET: math_test_current = 0; break;
        default: return false;
    }
    return true;    
}