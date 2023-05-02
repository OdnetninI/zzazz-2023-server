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
#include <time.h>

uint8_t server_id = 0; /* 0 = 13337, 1 = 13338, 2 = 13339*/
const char* server_boot[3] = {"boot/13337.boot.bin", "boot/13338.boot.bin", "boot/13339.boot.bin"};

#include "cpu.h"
#include "memory.h"
#include "syscall.h"
#include "instructions.h"
#include "opcode_table.h"

int main(int argc, char* argv[]) {
    /**
     * Which server should be run
     */ 
    if (argc != 2) {
        printf("Select a server 0 = 13337, 1 = 13338, 2 = 13339\n");
        return 1;
    }

    server_id = argv[1][0] - '0';
    if (server_id < 0 || server_id > 2) {
        printf("Select a server 0 = 13337, 1 = 13338, 2 = 13339\n");
        return 1;
    }

    /**
     * Init CPU registers and random seed
     */ 
    initCPU();

    /**
     * Load the BIOS
     */ 
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

    /**
     * Load the boot program
     */ 
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

    /**
     * Main Loop
     */ 
    while (true) {
        // prev_pc is for debugging, until all opcodes are implemented
        uint16_t prev_pc = cpu.PC;

        // Fetch the instruction
        uint8_t opcode = memory[cpu.PC];
        cpu.PC++;
        
        // Execute the instruction
        bool executed = instructions[opcode]();

        // Check if the instruction was executed
        if (!executed) {
            // Remove this when all opcodes are implemented
            printf("[%04x]Opcode %02x\n", prev_pc, opcode);
            printf("R0: %04x, R1: %04x, R2: %04x, R3: %04x, SP: %04x\n", cpu.R0, cpu.R1, cpu.R2, cpu.R3, cpu.SP);

            return 1;
        }
    }

    return 0;
}