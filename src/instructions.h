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

/* Forward declarations */
typedef bool inst_function();
inst_function *instructions[0x100];
inst_function * const original_instructions[0x100];

/**
 * Stack operations
*/
void push_16(uint16_t value) {
    cpu.SP -= 2;
    mem_write_16(cpu.SP, value);
}

uint16_t pop_16() {
    uint16_t value = mem_read_16(cpu.SP);
    cpu.SP += 2;
    return value;
}

void push(uint8_t value) {
    cpu.SP -= 1;
    mem_write(cpu.SP, value);
}

uint8_t pop() {
    uint8_t value = mem_read(cpu.SP);
    cpu.SP += 1;
    return value;
}

/**
 * Compare
*/
void cmp(uint16_t a, uint16_t b) {
    cpu.flag_z = false;
    cpu.flag_l = false;
    cpu.flag_g = false;
    if (a == b) cpu.flag_z = true;
    if (a < b) cpu.flag_l = true;
    if (a > b) cpu.flag_g = true;
}

/**
 * Opcode helper
*/
uint16_t read_op_imm() {
    uint16_t imm = mem_read_16(cpu.PC);
    cpu.PC += 2;
    return imm;
}

uint32_t mix_rngseed = 0;
void mix_srand(uint32_t seed) {
    mix_rngseed = seed;
}

uint16_t mix_rand() {
    mix_rngseed = 214013 * mix_rngseed + 2531011;
    return (mix_rngseed >> 16) & 0x7fff;
}

/**
 * Opcodes
*/
bool inst_break() {
    push_16(cpu.PC);
    cpu.PC = 0xfff0;
    return true;
}

bool inst_mul_r0_imm() {
    uint16_t value = read_op_imm();
    cpu.R0 = cpu.R0 * value;
    return true;
}

bool inst_mul_r0_r1() {
    cpu.R0 = cpu.R0 * cpu.R1;
    return true;
}

bool inst_div_r0_imm() {
    uint16_t value = read_op_imm();
    cpu.R0 = cpu.R0 / value;
    return true;
}

bool inst_div_r0_r1() {
    cpu.R0 = cpu.R0 / cpu.R1;
    return true;
}

bool inst_ret() {
    cpu.PC = pop_16();
    return true;
}

bool inst_syscall() {
    uint8_t syscall_id = mem_read(cpu.PC);
    cpu.PC++;
    return syscall(syscall_id);
}

bool inst_mix() {   
    mix_srand(cpu.mixed_instructions_level);
    cpu.mixed_instructions_level++;

    for (uint8_t op = 0xff; op > 0; --op) {
        uint8_t mixed_op = mix_rand() % op;
        inst_function* tmp = instructions[op];
        instructions[op] = instructions[mixed_op];
        instructions[mixed_op] = tmp;
    }
    return true;
}

bool inst_nop() {
    // NOP
    return true;
}

bool inst_mov_sp_imm() {
    cpu.SP = read_op_imm();
    return true;
}

bool inst_mov_r0_sp() {
    cpu.R0 = cpu.SP;
    return true;
}

bool inst_mov_r1_sp() {
    cpu.R1 = cpu.SP;
    return true;
}

bool inst_unmix() {
    for (uint16_t op = 0; op < 0x100; ++op) {
        instructions[op] = original_instructions[op];
    }
    cpu.mixed_instructions_level = 0;
    return true;
}

bool inst_illegal_0d() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_sll() {
    cpu.R0 = cpu.R0 << 1;
    return true;
}

bool inst_slr() {
    cpu.R0 = cpu.R0 >> 1;
    return true;
}

bool inst_mov_r0_imm() {
    cpu.R0 = read_op_imm();
    return true;
}

bool inst_mov_r1_imm() {
    cpu.R1 = read_op_imm();
    return true;
}

bool inst_mov_r2_imm() {
    cpu.R2 = read_op_imm();
    return true;
}

bool inst_mov_r3_imm() {
    cpu.R3 = read_op_imm();
    return true;
}

bool inst_illegal_14() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_15() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_16() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_17() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_18() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_19() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_1a() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_1b() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_1c() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_1d() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_1e() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_1f() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_mov_r0_r0() {
    cpu.R0 = cpu.R0;
    return true;
}

bool inst_mov_r0_r1() {
    cpu.R0 = cpu.R1;
    return true;
}

bool inst_mov_r0_r2() {
    cpu.R0 = cpu.R2;
    return true;
}

bool inst_mov_r0_r3() {
    cpu.R0 = cpu.R3;
    return true;
}

bool inst_mov_r1_r0() {
    cpu.R1 = cpu.R0;
    return true;
}

bool inst_mov_r1_r1() {
    cpu.R1 = cpu.R1;
    return true;
}

bool inst_mov_r1_r2() {
    cpu.R1 = cpu.R2;
    return true;
}

bool inst_mov_r1_r3() {
    cpu.R1 = cpu.R2;
    return true;
}

bool inst_mov_r2_r0() {
    cpu.R2 = cpu.R0;
    return true;
}

bool inst_mov_r2_r1() {
    cpu.R2 = cpu.R1;
    return true;
}

bool inst_mov_r2_r2() {
    cpu.R2 = cpu.R2;
    return true;
}

bool inst_mov_r2_r3() {
    cpu.R2 = cpu.R3;
    return true;
}

bool inst_mov_r3_r0() {
    cpu.R3 = cpu.R0;
    return true;
}

bool inst_mov_r3_r1() {
    cpu.R3 = cpu.R1;
    return true;
}

bool inst_mov_r3_r2() {
    cpu.R3 = cpu.R2;
    return true;
}

bool inst_mov_r3_r3() {
    cpu.R3 = cpu.R3;
    return true;
}

bool inst_add_r0_r0() {
    cpu.R0 += cpu.R0;
    return true;
}

bool inst_add_r0_r1() {
    cpu.R0 += cpu.R1;
    return true;
}

bool inst_add_r0_r2() {
    cpu.R0 += cpu.R2;
    return true;
}

bool inst_add_r0_r3() {
    cpu.R0 += cpu.R3;
    return true;
}

bool inst_add_r1_r0() {
    cpu.R1 += cpu.R0;
    return true;
}

bool inst_add_r1_r1() {
    cpu.R1 += cpu.R1;
    return true;
}

bool inst_add_r1_r2() {
    cpu.R1 += cpu.R2;
    return true;
}

bool inst_add_r1_r3() {
    cpu.R1 += cpu.R3;
    return true;
}

bool inst_add_r2_r0() {
    cpu.R2 += cpu.R0;
    return true;
}

bool inst_add_r2_r1() {
    cpu.R2 += cpu.R1;
    return true;
}

bool inst_add_r2_r2() {
    cpu.R2 += cpu.R2;
    return true;
}

bool inst_add_r2_r3() {
    cpu.R2 += cpu.R3;
    return true;
}

bool inst_add_r3_r0() {
    cpu.R3 += cpu.R0;
    return true;
}

bool inst_add_r3_r1() {
    cpu.R3 += cpu.R1;
    return true;
}

bool inst_add_r3_r2() {
    cpu.R3 += cpu.R2;
    return true;
}

bool inst_add_r3_r3() {
    cpu.R3 += cpu.R3;
    return true;
}

bool inst_mov_r0_m0() {
    cpu.R0 = mem_read(cpu.R0);
    return true;
}

bool inst_mov_r0_m1() {
    cpu.R0 = mem_read(cpu.R1);
    return true;
}

bool inst_mov_r0_m2() {
    cpu.R0 = mem_read(cpu.R2);
    return true;
}

bool inst_mov_r0_m3() {
    cpu.R0 = mem_read(cpu.R3);
    return true;
}

bool inst_mov_r1_m0() {
    cpu.R1 = mem_read(cpu.R0);
    return true;
}

bool inst_mov_r1_m1() {
    cpu.R1 = mem_read(cpu.R1);
    return true;
}

bool inst_mov_r1_m2() {
    cpu.R1 = mem_read(cpu.R2);
    return true;
}

bool inst_mov_r1_m3() {
    cpu.R1 = mem_read(cpu.R3);
    return true;
}

bool inst_mov_r2_m0() {
    cpu.R2 = mem_read(cpu.R0);
    return true;
}

bool inst_mov_r2_m1() {
    cpu.R2 = mem_read(cpu.R1);
    return true;
}

bool inst_mov_r2_m2() {
    cpu.R2 = mem_read(cpu.R2);
    return true;
}

bool inst_mov_r2_m3() {
    cpu.R2 = mem_read(cpu.R3);
    return true;
}

bool inst_mov_r3_m0() {
    cpu.R3 = mem_read(cpu.R0);
    return true;
}

bool inst_mov_r3_m1() {
    cpu.R3 = mem_read(cpu.R1);
    return true;
}

bool inst_mov_r3_m2() {
    cpu.R3 = mem_read(cpu.R2);
    return true;
}

bool inst_mov_r3_m3() {
    cpu.R3 = mem_read(cpu.R3);
    return true;
}

bool inst_mov_r0_w0() {
    cpu.R0 = mem_read_16(cpu.R0);
    return true;
}

bool inst_mov_r0_w1() {
    cpu.R0 = mem_read_16(cpu.R1);
    return true;
}

bool inst_mov_r0_w2() {
    cpu.R0 = mem_read_16(cpu.R2);
    return true;
}

bool inst_mov_r0_w3() {
    cpu.R0 = mem_read_16(cpu.R3);
    return true;
}

bool inst_mov_r1_w0() {
    cpu.R1 = mem_read_16(cpu.R0);
    return true;
}

bool inst_mov_r1_w1() {
    cpu.R1 = mem_read_16(cpu.R1);
    return true;
}

bool inst_mov_r1_w2() {
    cpu.R1 = mem_read_16(cpu.R2);
    return true;
}

bool inst_mov_r1_w3() {
    cpu.R1 = mem_read_16(cpu.R3);
    return true;
}

bool inst_mov_r2_w0() {
    cpu.R2 = mem_read_16(cpu.R0);
    return true;
}

bool inst_mov_r2_w1() {
    cpu.R2 = mem_read_16(cpu.R1);
    return true;
}

bool inst_mov_r2_w2() {
    cpu.R2 = mem_read_16(cpu.R2);
    return true;
}

bool inst_mov_r2_w3() {
    cpu.R2 = mem_read_16(cpu.R3);
    return true;
}

bool inst_mov_r3_w0() {
    cpu.R3 = mem_read_16(cpu.R0);
    return true;
}

bool inst_mov_r3_w1() {
    cpu.R3 = mem_read_16(cpu.R1);
    return true;
}

bool inst_mov_r3_w2() {
    cpu.R3 = mem_read_16(cpu.R2);
    return true;
}

bool inst_mov_r3_w3() {
    cpu.R3 = mem_read_16(cpu.R3);
    return true;
}

bool inst_mov_m0_r0() {
    mem_write(cpu.R0, cpu.R0);
    return true;
}

bool inst_mov_m0_r1() {
    mem_write(cpu.R0, cpu.R1);
    return true;
}

bool inst_mov_m0_r2() {
    mem_write(cpu.R0, cpu.R2);
    return true;
}

bool inst_mov_m0_r3() {
    mem_write(cpu.R0, cpu.R3);
    return true;
}

bool inst_mov_m1_r0() {
    mem_write(cpu.R1, cpu.R0);
    return true;
}

bool inst_mov_m1_r1() {
    mem_write(cpu.R1, cpu.R1);
    return true;
}

bool inst_mov_m1_r2() {
    mem_write(cpu.R1, cpu.R2);
    return true;
}

bool inst_mov_m1_r3() {
    mem_write(cpu.R1, cpu.R3);
    return true;
}

bool inst_mov_m2_r0() {
    mem_write(cpu.R2, cpu.R0);
    return true;
}

bool inst_mov_m2_r1() {
    mem_write(cpu.R2, cpu.R1);
    return true;
}

bool inst_mov_m2_r2() {
    mem_write(cpu.R2, cpu.R2);
    return true;
}

bool inst_mov_m2_r3() {
    mem_write(cpu.R2, cpu.R3);
    return true;
}

bool inst_mov_m3_r0() {
    mem_write(cpu.R3, cpu.R0);
    return true;
}

bool inst_mov_m3_r1() {
    mem_write(cpu.R3, cpu.R1);
    return true;
}

bool inst_mov_m3_r2() {
    mem_write(cpu.R3, cpu.R2);
    return true;
}

bool inst_mov_m3_r3() {
    mem_write(cpu.R3, cpu.R3);
    return true;
}

bool inst_mov_w0_r0() {
    mem_write_16(cpu.R0, cpu.R0);
    return true;
}

bool inst_mov_w0_r1() {
    mem_write_16(cpu.R0, cpu.R1);
    return true;
}

bool inst_mov_w0_r2() {
    mem_write_16(cpu.R0, cpu.R2);
    return true;
}

bool inst_mov_w0_r3() {
    mem_write_16(cpu.R0, cpu.R3);
    return true;
}

bool inst_mov_w1_r0() {
    mem_write_16(cpu.R1, cpu.R0);
    return true;
}

bool inst_mov_w1_r1() {
    mem_write_16(cpu.R1, cpu.R1);
    return true;
}

bool inst_mov_w1_r2() {
    mem_write_16(cpu.R1, cpu.R2);
    return true;
}

bool inst_mov_w1_r3() {
    mem_write_16(cpu.R1, cpu.R3);
    return true;
}

bool inst_mov_w2_r0() {
    mem_write_16(cpu.R2, cpu.R0);
    return true;
}

bool inst_mov_w2_r1() {
    mem_write_16(cpu.R2, cpu.R1);
    return true;
}

bool inst_mov_w2_r2() {
    mem_write_16(cpu.R2, cpu.R2);
    return true;
}

bool inst_mov_w2_r3() {
    mem_write_16(cpu.R2, cpu.R3);
    return true;
}

bool inst_mov_w3_r0() {
    mem_write_16(cpu.R3, cpu.R0);
    return true;
}

bool inst_mov_w3_r1() {
    mem_write_16(cpu.R3, cpu.R1);
    return true;
}

bool inst_mov_w3_r2() {
    mem_write_16(cpu.R3, cpu.R2);
    return true;
}

bool inst_mov_w3_r3() {
    mem_write_16(cpu.R3, cpu.R3);
    return true;
}

bool inst_cmp_r0_r0() {
    cmp(cpu.R0, cpu.R0);
    return true;
}

bool inst_cmp_r0_r1() {
    cmp(cpu.R0, cpu.R1);
    return true;
}

bool inst_cmp_r0_r2() {
    cmp(cpu.R0, cpu.R2);
    return true;
}

bool inst_cmp_r0_r3() {
    cmp(cpu.R0, cpu.R3);
    return true;
}

bool inst_cmp_r1_r0() {
    cmp(cpu.R1, cpu.R0);
    return true;
}

bool inst_cmp_r1_r1() {
    cmp(cpu.R1, cpu.R1);
    return true;
}

bool inst_cmp_r1_r2() {
    cmp(cpu.R1, cpu.R2);
    return true;
}

bool inst_cmp_r1_r3() {
    cmp(cpu.R1, cpu.R3);
    return true;
}

bool inst_cmp_r2_r0() {
    cmp(cpu.R2, cpu.R0);
    return true;
}

bool inst_cmp_r2_r1() {
    cmp(cpu.R2, cpu.R1);
    return true;
}

bool inst_cmp_r2_r2() {
    cmp(cpu.R2, cpu.R2);
    return true;
}

bool inst_cmp_r2_r3() {
    cmp(cpu.R2, cpu.R3);
    return true;
}

bool inst_cmp_r3_r0() {
    cmp(cpu.R3, cpu.R0);
    return true;
}

bool inst_cmp_r3_r1() {
    cmp(cpu.R3, cpu.R1);
    return true;
}

bool inst_cmp_r3_r2() {
    cmp(cpu.R3, cpu.R2);
    return true;
}

bool inst_cmp_r3_r3() {
    cmp(cpu.R3, cpu.R3);
    return true;
}

bool inst_push_r0() {
    push_16(cpu.R0);
    return true;
}

bool inst_push_r1() {
    push_16(cpu.R1);
    return true;
}

bool inst_push_r2() {
    push_16(cpu.R2);
    return true;
}

bool inst_push_r3() {
    push_16(cpu.R3);
    return true;
}

bool inst_pop_r0() {
    cpu.R0 = pop_16();
    return true;
}

bool inst_pop_r1() {
    cpu.R1 = pop_16();
    return true;
}

bool inst_pop_r2() {
    cpu.R2 = pop_16();
    return true;
}

bool inst_pop_r3() {
    cpu.R3 = pop_16();
    return true;
}

bool inst_jump_imm() {
    uint16_t dest_addr = mem_read_16(cpu.PC);
    cpu.PC = dest_addr;
    return true;
}

bool inst_call_imm() {
    uint16_t dest_addr = read_op_imm();

    push_16(cpu.PC);

    cpu.PC = dest_addr;
    return true;
}

bool inst_jlt_imm() {
    uint16_t value = read_op_imm();

    if (cpu.flag_l) cpu.PC = value;
    return true;
}

bool inst_jgt_imm() {
    uint16_t value = read_op_imm();
    
    if (cpu.flag_g) cpu.PC = value;
    return true;
}

bool inst_jeq_imm() {
    uint16_t value = read_op_imm();
    
    if (cpu.flag_z) cpu.PC = value;
    return true;
}

bool inst_jne_imm() {
    uint16_t value = read_op_imm();
    
    if (!cpu.flag_z) cpu.PC = value;
    return true;
}

bool inst_clt_imm() {
    uint16_t dest_addr = read_op_imm();

    if (cpu.flag_l) {
        push_16(cpu.PC);
        cpu.PC = dest_addr;
    }
    return true;
}

bool inst_cgt_imm() {
    uint16_t dest_addr = read_op_imm();

    if (cpu.flag_g) {
        push_16(cpu.PC);
        cpu.PC = dest_addr;
    }
    return true;
}

bool inst_ceq_imm() {
    uint16_t dest_addr = read_op_imm();

    if (cpu.flag_z) {
        push_16(cpu.PC);
        cpu.PC = dest_addr;
    }
    return true;
}

bool inst_cne_imm() {
    uint16_t dest_addr = read_op_imm();

    if (!cpu.flag_z) {
        push_16(cpu.PC);
        cpu.PC = dest_addr;
    }
    return true;
}

bool inst_cmp_r0_imm() {
    uint16_t value = read_op_imm();
    cmp(cpu.R0, value);
    return true;
}

bool inst_cmp_r1_imm() {
    uint16_t value = read_op_imm();
    cmp(cpu.R1, value);
    return true;
}

bool inst_cmp_r2_imm() {
    uint16_t value = read_op_imm();
    cmp(cpu.R2, value);
    return true;
}

bool inst_cmp_r3_imm() {
    uint16_t value = read_op_imm();
    cmp(cpu.R3, value);
    return true;
}

bool inst_push_sp() {
    push_16(cpu.SP);
    return true;
}

bool inst_push_pc() {
    push_16(cpu.PC - 1);
    return true;
}

bool inst_inc_r0() {
    cpu.R0++;
    return true;
}

bool inst_inc_r1() {
    cpu.R1++;
    return true;
}

bool inst_inc_r2() {
    cpu.R2++;
    return true;
}

bool inst_inc_r3() {
    cpu.R3++;
    return true;
}

bool inst_dec_r0() {
    cpu.R0--;
    return true;
}

bool inst_dec_r1() {
    cpu.R1--;
    return true;
}

bool inst_dec_r2() {
    cpu.R2--;
    return true;
}

bool inst_dec_r3() {
    cpu.R3--;
    return true;
}

bool inst_mov_r0_mimm() {
    uint16_t address = read_op_imm();
    cpu.R0 = mem_read(address);
    return true;
}

bool inst_mov_r1_mimm() {
    uint16_t address = read_op_imm();
    cpu.R1 = mem_read(address);
    return true;
}

bool inst_mov_r2_mimm() {
    uint16_t address = read_op_imm();
    cpu.R2 = mem_read(address);
    return true;
}

bool inst_mov_r3_mimm() {
    uint16_t address = read_op_imm();
    cpu.R3 = mem_read(address);
    return true;
}

bool inst_mov_r0_wimm() {
    uint16_t address = read_op_imm();
    cpu.R0 = mem_read_16(address);
    return true;
}

bool inst_mov_r1_wimm() {
    uint16_t address = read_op_imm();
    cpu.R1 = mem_read_16(address);
    return true;
}

bool inst_mov_r2_wimm() {
    uint16_t address = read_op_imm();
    cpu.R2 = mem_read_16(address);
    return true;
}

bool inst_mov_r3_wimm() {
    uint16_t address = read_op_imm();
    cpu.R3 = mem_read_16(address);
    return true;
}

bool inst_mov_mimm_r0() {
    uint16_t address = read_op_imm();
    mem_write(address, cpu.R0 & 0xff);
    return true;
}

bool inst_mov_mimm_r1() {
    uint16_t address = read_op_imm();
    mem_write(address, cpu.R1 & 0xff);
    return true;
}

bool inst_mov_mimm_r2() {
    uint16_t address = read_op_imm();
    mem_write(address, cpu.R2 & 0xff);
    return true;
}

bool inst_mov_mimm_r3() {
    uint16_t address = read_op_imm();
    mem_write(address, cpu.R3 & 0xff);
    return true;
}

bool inst_mov_wimm_r0() {
    uint16_t address = read_op_imm();
    mem_write_16(address, cpu.R0);
    return true;
}

bool inst_mov_wimm_r1() {
    uint16_t address = read_op_imm();
    mem_write_16(address, cpu.R1);
    return true;
}

bool inst_mov_wimm_r2() {
    uint16_t address = read_op_imm();
    mem_write_16(address, cpu.R2);
    return true;
}

bool inst_mov_wimm_r3() {
    uint16_t address = read_op_imm();
    mem_write_16(address, cpu.R3);
    return true;
}

bool inst_illegal_c0() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_c1() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_c2() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_c3() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_push_imm() {
    uint16_t value = read_op_imm();
    push_16(value);
    cpu.PC += 2;
    return true;
}

bool inst_jump_r0() {
    cpu.PC = cpu.R0;
    return true;
}

bool inst_jump_r1() {
    cpu.PC = cpu.R1;
    return true;
}

bool inst_jump_r2() {
    cpu.PC = cpu.R2;
    return true;
}

bool inst_jump_r3() {
    cpu.PC = cpu.R3;
    return true;
}

bool inst_call_r0() {
    push_16(cpu.PC);
    cpu.PC = cpu.R0;
    return true;
}

bool inst_call_r1() {
    push_16(cpu.PC);
    cpu.PC = cpu.R1;
    return true;
}

bool inst_call_r2() {
    push_16(cpu.PC);
    cpu.PC = cpu.R2;
    return true;
}

bool inst_call_r3() {
    push_16(cpu.PC);
    cpu.PC = cpu.R3;
    return true;
}

bool inst_and_r0_imm() {
    uint16_t value = read_op_imm();
    cpu.R0 = cpu.R0 & value;
    return true;
}

bool inst_or_r0_imm() {
    uint16_t value = read_op_imm();
    cpu.R0 = cpu.R0 | value;
    return true;
}

bool inst_xor_r0_imm() {
    uint16_t value = read_op_imm();
    cpu.R0 = cpu.R0 ^ value;
    return true;
}

bool inst_and_r0_r0() {
    cpu.R0 = cpu.R0 & cpu.R0;
    return true;
}

bool inst_and_r0_r1() {
    cpu.R0 = cpu.R0 & cpu.R1;
    return true;
}

bool inst_and_r0_r2() {
    cpu.R0 = cpu.R0 & cpu.R2;
    return true;
}

bool inst_and_r0_r3() {
    cpu.R0 = cpu.R0 & cpu.R3;
    return true;
}

bool inst_or_r0_r0() {
    cpu.R0 = cpu.R0 | cpu.R0;
    return true;
}

bool inst_or_r0_r1() {
    cpu.R0 = cpu.R0 | cpu.R1;
    return true;
}

bool inst_or_r0_r2() {
    cpu.R0 = cpu.R0 | cpu.R2;
    return true;
}

bool inst_or_r0_r3() {
    cpu.R0 = cpu.R0 | cpu.R3;
    return true;
}

bool inst_xor_r0_r0() {
    cpu.R0 = cpu.R0 ^ cpu.R0;
    return true;
}

bool inst_xor_r0_r1() {
    cpu.R0 = cpu.R0 ^ cpu.R1;
    return true;
}

bool inst_xor_r0_r2() {
    cpu.R0 = cpu.R0 ^ cpu.R2;
    return true;
}

bool inst_xor_r0_r3() {
    cpu.R0 = cpu.R0 ^ cpu.R3;
    return true;
}

bool inst_swap_r0() {
    cpu.R0 = (cpu.R0 >> 8) | (cpu.R0 << 8);
    return true;
}

bool inst_swap_r1() {
    cpu.R1 = (cpu.R1 >> 8) | (cpu.R1 << 8);
    return true;
}

bool inst_swap_r2() {
    cpu.R2 = (cpu.R2 >> 8) | (cpu.R2 << 8);
    return true;
}

bool inst_swap_r3() {
    cpu.R3 = (cpu.R3 >> 8) | (cpu.R3 << 8);
    return true;
}

bool inst_add_r0_imm() {
    uint16_t value = read_op_imm();
    cpu.R0 += value;
    return true;
}

bool inst_add_r1_imm() {
    uint16_t value = read_op_imm();
    cpu.R1 += value;
    return true;
}

bool inst_add_r2_imm() {
    uint16_t value = read_op_imm();
    cpu.R2 += value;
    return true;
}

bool inst_add_r3_imm() {
    uint16_t value = read_op_imm();
    cpu.R3 += value;
    return true;
}

bool inst_illegal_e4() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_e5() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_e6() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_e7() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_e8() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_e9() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_ea() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_eb() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_ec() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_ed() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_ee() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_ef() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_f0() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_f1() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_f2() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_f3() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_f4() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_f5() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_f6() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_f7() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_f8() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_f9() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_fa() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_fb() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_fc() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_fd() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_fe() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool inst_illegal_ff() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}
