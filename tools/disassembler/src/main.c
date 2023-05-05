#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define FILE_IMP
#include "file.h"

uint16_t entry_point = 0x2000;
uint16_t pc;

void write_inst(const char* inst) {
    fprintf(output, "%s", inst);
}

void write_inst_imm(const char* inst) {
    uint16_t imm = ((uint16_t)fgetc(input)) | (((uint16_t)fgetc(input)) << 8);
    fprintf(output, "%s #%04x", inst, imm);
    pc += 2;
}

void write_inst_imm_addr(const char* inst) {
    uint16_t imm = ((uint16_t)fgetc(input)) | (((uint16_t)fgetc(input)) << 8);
    fprintf(output, "%s, [#%04x]", inst, imm);
    pc += 2;
}

void write_sys() {
    uint16_t imm = ((uint16_t)fgetc(input));
    fprintf(output, "sys #%02x", imm);
    pc++;
}

void write_int_regs_in_order(uint8_t op, char* inst) {
    int reg = (op & 0xc) >> 2;
    int regm = op & 0x3;
    fprintf(output, "%s %%r%d, %%r%d", inst, reg, regm);
}

void write_int_regs_in_reverse(uint8_t op, char* inst) {
    int reg = (op & 0xc) >> 2;
    int regm = op & 0x3;
    fprintf(output, "%s %%r%d, %%r%d", inst, regm, reg);
}

void write_int_regs_in_order_addr(uint8_t op, char* inst) {
    int reg = (op & 0xc) >> 2;
    int regm = op & 0x3;
    fprintf(output, "%s %%r%d, [%%r%d]", inst, reg, regm);
}

void write_int_regs_in_reverse_addr(uint8_t op, char* inst) {
    int reg = (op & 0xc) >> 2;
    int regm = op & 0x3;
    fprintf(output, "%s %%r%d, [%%r%d]", inst, regm, reg);
}

int main(int argc, char* argv[]) {
    if (argc < 3) return 1;

    if (argc == 4) {
        uint64_t ep = strtol(argv[3], NULL, 16);
        if (ep < 0x1000) printf("Warning: Memory space from 0x0000 to 0xffff is protected by BIOS\n");
        if (ep > 0xffff) { printf("Error, entry point must be between 0x0000 and 0xffff\n"); return 1; }
        entry_point = ep & 0xffff;
    }
    
    input = fopen(argv[1], "r");
    if (!input) {
        printf("Error: %s not found\n", argv[1]);
        return 1;
    }

    output = fopen(argv[2], "w");
    if (!output) {
        printf("Error: Cannot write %s\n", argv[2]);
        return 1;
    }

    fprintf(output, "entry #%04x ; Entry point of the program\n", entry_point);
    pc = entry_point;

    while(!feof(input)) {
        uint16_t opcode_pc = pc;
        uint8_t opcode = fgetc(input);
        fprintf(output, "\n\t");
        pc++;

            switch(opcode) {
                case 0x00: write_inst("break");               break;
                case 0x01: write_inst_imm("mul %r0");         break;
                case 0x02: write_inst("mul %r0, %r1");        break;
                case 0x03: write_inst_imm("div %r0");         break;
                case 0x04: write_inst("div %r0, %r1");        break;
                case 0x05: write_inst("ret");                 break;
                case 0x06: write_sys();                       break;
                case 0x07: write_inst("zzazz");               break;
                case 0x08: write_inst("nop");                 break;
                case 0x09: write_inst_imm("mov %sp");         break;
                case 0x0a: write_inst("mov %r0, %sp");        break;
                case 0x0b: write_inst("div %r1, %sp");        break;
                case 0x0c: write_inst("nopc");                break;
                case 0x0d: write_inst("illegal");             break;
                case 0x0e: write_inst("lls %r0");             break;
                case 0x0f: write_inst("rls %r0");             break;

                case 0x10: write_inst_imm("mov %r0");         break;
                case 0x11: write_inst_imm("mov %r1");         break;
                case 0x12: write_inst_imm("mov %r2");         break;
                case 0x13: write_inst_imm("mov %r3");         break;
                case 0x14 ... 0x1f: 
                    write_inst("illegal");
                    break;

                case 0x20 ... 0x2f:
                    write_int_regs_in_order(opcode, "mov");        
                    break;

                case 0x30 ... 0x3f:
                    write_int_regs_in_order(opcode, "add");        
                    break;

                case 0x40 ... 0x4f:
                    write_int_regs_in_order_addr(opcode, "ldb");        
                    break;

                case 0x50 ... 0x5f:
                    write_int_regs_in_order_addr(opcode, "ld");        
                    break;

                case 0x60 ... 0x6f:
                    write_int_regs_in_reverse_addr(opcode, "stb");        
                    break;

                case 0x70 ... 0x7f:
                    write_int_regs_in_reverse_addr(opcode, "st");        
                    break;

                case 0x80 ... 0x8f:
                    write_int_regs_in_order(opcode, "cmp");        
                    break;
                
                case 0x90: write_inst("push %r0"); break;
                case 0x91: write_inst("push %r1"); break;
                case 0x92: write_inst("push %r2"); break;
                case 0x93: write_inst("push %r3"); break;
                case 0x94: write_inst("pop %r0"); break;
                case 0x95: write_inst("pop %r1"); break;
                case 0x96: write_inst("pop %r2"); break;
                case 0x97: write_inst("pop %r3"); break;
                case 0x98: write_inst_imm("jump");         break;
                case 0x99: write_inst_imm("call");         break;
                case 0x9a: write_inst_imm("j.l");         break;
                case 0x9b: write_inst_imm("j.g");         break;
                case 0x9c: write_inst_imm("j.z");         break;
                case 0x9d: write_inst_imm("j.n");         break;
                case 0x9e: write_inst_imm("call.l");         break;
                case 0x9f: write_inst_imm("call.g");         break;

                case 0xa0: write_inst_imm("call.z");         break;
                case 0xa1: write_inst_imm("call.n");         break;
                case 0xa2: write_inst_imm("cmp %r0");         break;
                case 0xa3: write_inst_imm("cmp %r1");         break;
                case 0xa4: write_inst_imm("cmp %r2");         break;
                case 0xa5: write_inst_imm("cmp %r3");         break;
                case 0xa6: write_inst("push %sp");         break;
                case 0xa7: write_inst("push %pc");         break;
                case 0xa8: write_inst("inc %r0");         break;
                case 0xa9: write_inst("inc %r1");         break;
                case 0xaa: write_inst("inc %r2");         break;
                case 0xab: write_inst("inc %r3");         break;
                case 0xac: write_inst("dec %r0");         break;
                case 0xad: write_inst("dec %r1");         break;
                case 0xae: write_inst("dec %r2");         break;
                case 0xaf: write_inst("dec %r3");         break;

                case 0xb0: write_inst_imm_addr("ldb %r0");         break;
                case 0xb1: write_inst_imm_addr("ldb %r1");         break;
                case 0xb2: write_inst_imm_addr("ldb %r2");         break;
                case 0xb3: write_inst_imm_addr("ldb %r3");         break;
                case 0xb4: write_inst_imm_addr("ld %r0");         break;
                case 0xb5: write_inst_imm_addr("ld %r1");         break;
                case 0xb6: write_inst_imm_addr("ld %r2");         break;
                case 0xb7: write_inst_imm_addr("ld %r3");         break;
                case 0xb8: write_inst_imm_addr("stb %r0");         break;
                case 0xb9: write_inst_imm_addr("stb %r1");         break;
                case 0xba: write_inst_imm_addr("stb %r2");         break;
                case 0xbb: write_inst_imm_addr("stb %r3");         break;
                case 0xbc: write_inst_imm_addr("st %r0");         break;
                case 0xbd: write_inst_imm_addr("st %r1");         break;
                case 0xbe: write_inst_imm_addr("st %r2");         break;
                case 0xbf: write_inst_imm_addr("st %r3");         break;

                case 0xc0 ... 0xc4: 
                    write_inst("illegal");
                    break;
                case 0xc5: write_inst("jump %r0"); break;
                case 0xc6: write_inst("jump %r1"); break;
                case 0xc7: write_inst("jump %r2"); break;
                case 0xc8: write_inst("jump %r3"); break;
                case 0xc9: write_inst("call %r0"); break;
                case 0xca: write_inst("call %r1"); break;
                case 0xcb: write_inst("call %r2"); break;
                case 0xcc: write_inst("call %r3"); break;
                case 0xcd: write_inst_imm("and %r0");         break;
                case 0xce: write_inst_imm("or %r0");         break;
                case 0xcf: write_inst_imm("xor %r0");         break;

                case 0xd0: write_inst("and %r0, %r1"); break;
                case 0xd1: write_inst("and %r0, %r1"); break;
                case 0xd2: write_inst("and %r0, %r2"); break;
                case 0xd3: write_inst("and %r0, %r3"); break;
                case 0xd4: write_inst("or %r0, %r1"); break;
                case 0xd5: write_inst("or %r0, %r1"); break;
                case 0xd6: write_inst("or %r0, %r2"); break;
                case 0xd7: write_inst("or %r0, %r3"); break;
                case 0xd8: write_inst("xor %r0, %r1"); break;
                case 0xda: write_inst("xor %r0, %r1"); break;
                case 0xdb: write_inst("xor %r0, %r2"); break;
                case 0xdc: write_inst("xor %r0, %r3"); break;
                case 0xdd ... 0xdf:
                    write_inst("swap %r0"); 
                    break;

                case 0xe0: write_inst_imm("add %r0");         break;
                case 0xe1: write_inst_imm("add %r1");         break;
                case 0xe2: write_inst_imm("add %r2");         break;
                case 0xe3: write_inst_imm("add %r3");         break;
                case 0xe4 ... 0xff: 
                    write_inst("illegal");
                    break;
            }
        fprintf(output, " ; @%04x opcode %02x ", opcode_pc, opcode);
    }

    fclose(input);
    fclose(output);
    return 0;
}