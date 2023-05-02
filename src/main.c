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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

uint8_t server_id = 0; /* 0 = 13337, 1 = 13338, 2 = 13339*/
const char* server_boot[3] = {"boot/13337.boot.bin", "boot/13338.boot.bin", "boot/13339.boot.bin"};

uint8_t memory[0xffff] = {0};
uint16_t pc = 0xf000;

uint16_t R0;
uint16_t R1;
uint16_t R2;
uint16_t R3;
uint16_t SP;

bool flag_z = false;
bool flag_l = false;
bool flag_g = false;

uint8_t mem_read(uint16_t addr) {
    if (addr < 0x1000 && pc >= 0x1000) return 0x55;
    return memory[addr];
}

void mem_write(uint16_t addr, uint8_t data) {
    if (addr < 0x1000 && pc >= 0x1000) return;
    memory[addr] = data;
}

uint16_t mem_read_16(uint16_t addr) {
    uint8_t low = mem_read(addr);
    uint8_t high = mem_read(addr + 1);
    return ((uint16_t)high << 8) | ((uint16_t)low);
}

void mem_write_16(uint16_t addr, uint16_t data) {
    uint8_t low = data & 0xff;
    uint16_t high = (data >> 8) & 0xff;
    mem_write(addr, low);
    mem_write(addr + 1, high);
}

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
#define math_test_max 1

struct _math_test {
    uint16_t question;
    uint16_t answer;
};

struct _math_test math_test[math_test_max] = {
    {0, 0},
};

bool syscall(uint8_t id) {
    switch (id) {
        case 0x01: printf("%c", R0 & 0xff); break;
        case 0x02:
            char c = 0;
            scanf("%c", &c);
            R0 = c;
            break;
        case 0x03: printf("Exit\n"); exit(0);
        case 0x04: read_file(R1, R0); break;
        case 0x05: R0 = rand(); break;


        case 0x64: R0 = math_test[math_test_current].question; break;
        case 0x65: 
            if (R0 == math_test[math_test_current].answer) {
                if(math_test_current + 1 == math_test_max) {
                    printf("Test successful! FOOLS2023_{UnknownArchitectureMath}\n");
                }
                else {
                    math_test_current++;
                }
            }
            break;
        case 0x66: math_test_current = 0; break;
        default: return false;
    }
    return true;    
}

#include "instructions.h"

typedef bool inst_function();
inst_function *instructions[0x100] = {
    op_00,op_01,op_02,op_03,op_04,op_05,op_06,op_07,op_08,op_09,op_0a,op_0b,op_0c,op_0d,op_0e,op_0f,
    op_10,op_11,op_12,op_13,op_14,op_15,op_16,op_17,op_18,op_19,op_1a,op_1b,op_1c,op_1d,op_1e,op_1f,
    op_20,op_21,op_22,op_23,op_24,op_25,op_26,op_27,op_28,op_29,op_2a,op_2b,op_2c,op_2d,op_2e,op_2f,
    op_30,op_31,op_32,op_33,op_34,op_35,op_36,op_37,op_38,op_39,op_3a,op_3b,op_3c,op_3d,op_3e,op_3f,
    op_40,op_41,op_42,op_43,op_44,op_45,op_46,op_47,op_48,op_49,op_4a,op_4b,op_4c,op_4d,op_4e,op_4f,
    op_50,op_51,op_52,op_53,op_54,op_55,op_56,op_57,op_58,op_59,op_5a,op_5b,op_5c,op_5d,op_5e,op_5f,
    op_60,op_61,op_62,op_63,op_64,op_65,op_66,op_67,op_68,op_69,op_6a,op_6b,op_6c,op_6d,op_6e,op_6f,
    op_70,op_71,op_72,op_73,op_74,op_75,op_76,op_77,op_78,op_79,op_7a,op_7b,op_7c,op_7d,op_7e,op_7f,
    op_80,op_81,op_82,op_83,op_84,op_85,op_86,op_87,op_88,op_89,op_8a,op_8b,op_8c,op_8d,op_8e,op_8f,
    op_90,op_91,op_92,op_93,op_94,op_95,op_96,op_97,op_98,op_99,op_9a,op_9b,op_9c,op_9d,op_9e,op_9f,
    op_a0,op_a1,op_a2,op_a3,op_a4,op_a5,op_a6,op_a7,op_a8,op_a9,op_aa,op_ab,op_ac,op_ad,op_ae,op_af,
    op_b0,op_b1,op_b2,op_b3,op_b4,op_b5,op_b6,op_b7,op_b8,op_b9,op_ba,op_bb,op_bc,op_bd,op_be,op_bf,
    op_c0,op_c1,op_c2,op_c3,op_c4,op_c5,op_c6,op_c7,op_c8,op_c9,op_ca,op_cb,op_cc,op_cd,op_ce,op_cf,
    op_d0,op_d1,op_d2,op_d3,op_d4,op_d5,op_d6,op_d7,op_d8,op_d9,op_da,op_db,op_dc,op_dd,op_de,op_df,
    op_e0,op_e1,op_e2,op_e3,op_e4,op_e5,op_e6,op_e7,op_e8,op_e9,op_ea,op_eb,op_ec,op_ed,op_ee,op_ef,
    op_f0,op_f1,op_f2,op_f3,op_f4,op_f5,op_f6,op_f7,op_f8,op_f9,op_fa,op_fb,op_fc,op_fd,op_fe,op_ff,
};

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Select a server 0 = 13337, 1 = 13338, 2 = 13339\n");
        return 1;
    }

    server_id = argv[1][0] - '0';
    if (server_id < 0 || server_id > 2) {
        printf("Select a server 0 = 13337, 1 = 13338, 2 = 13339\n");
        return 1;
    }

    srand(42);

    FILE* f_bios = fopen("bios/bios.v1.3.bin", "r");
    if (!f_bios) {
        printf("BIOS: %s not found\n", "bios/bios.v1.3.bin");
        return 1;
    }
    fseek(f_bios, 0, SEEK_END);
    uint16_t f_bios_size = ftell(f_bios);
    rewind(f_bios);
    fread(&(memory[0x0000]), f_bios_size, 1, f_bios);
    fclose(f_bios);

    FILE* f_boot = fopen(server_boot[server_id], "r");
    if (!f_boot) {
        printf("BOOT: %s not found\n", server_boot[server_id]);
        return 1;
    }
    fseek(f_boot, 0, SEEK_END);
    uint16_t f_boot_size = ftell(f_boot);
    rewind(f_boot);
    fread(&(memory[0xf000]), f_boot_size, 1, f_boot);
    fclose(f_boot);

    while (true) {
        uint16_t prev_pc = pc;

        uint8_t opcode = memory[pc];
        pc++;
        
        bool executed = instructions[opcode]();
        if (!executed) {
            printf("[%04x]Opcode %02x\n", prev_pc, opcode);
            printf("R0: %04x, R1: %04x, R2: %04x, R3: %04x, SP: %04x\n", R0, R1, R2, R3, SP);
            return 1;
        }
    }

    return 0;
}