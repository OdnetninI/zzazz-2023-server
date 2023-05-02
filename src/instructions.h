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

/**
 * Opcodes
*/
bool op_00() {
    push_16(cpu.PC);
    cpu.PC = 0xfff0;
    return true;
}

bool op_01() {
    uint16_t value = read_op_imm();
    cpu.R0 = cpu.R0 * value;
    return true;
}

bool op_02() {
    cpu.R0 = cpu.R0 * cpu.R1;
    return true;
}

bool op_03() {
    uint16_t value = read_op_imm();
    cpu.R0 = cpu.R0 / value;
    return true;
}

bool op_04() {
    cpu.R0 = cpu.R0 / cpu.R1;
    return true;
}

bool op_05() {
    cpu.PC = pop_16();
    return true;
}

bool op_06() {
    uint8_t syscall_id = mem_read(cpu.PC);
    cpu.PC++;
    return syscall(syscall_id);
}

bool op_07() {
    printf("ZZAZZ is trolling us\n");
    return false;
}

bool op_08() {
    // NOP
    return true;
}

bool op_09() {
    cpu.SP = read_op_imm();
    return true;
}

bool op_0a() {
    cpu.R0 = cpu.SP;
    return true;
}

bool op_0b() {
    cpu.R1 = cpu.SP;
    return true;
}

bool op_0c() {
    // NOP
    return true;
}

bool op_0d() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_0e() {
    cpu.R0 = cpu.R0 << 1;
    return true;
}

bool op_0f() {
    cpu.R0 = cpu.R0 >> 1;
    return true;
}

bool op_10() {
    cpu.R0 = read_op_imm();
    return true;
}

bool op_11() {
    cpu.R1 = read_op_imm();
    return true;
}

bool op_12() {
    cpu.R2 = read_op_imm();
    return true;
}

bool op_13() {
    cpu.R3 = read_op_imm();
    return true;
}

bool op_14() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_15() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_16() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_17() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_18() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_19() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_1a() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_1b() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_1c() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_1d() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_1e() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_1f() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_20() {
    cpu.R0 = cpu.R0;
    return true;
}

bool op_21() {
    cpu.R0 = cpu.R1;
    return true;
}

bool op_22() {
    cpu.R0 = cpu.R2;
    return true;
}

bool op_23() {
    cpu.R0 = cpu.R3;
    return true;
}

bool op_24() {
    cpu.R1 = cpu.R0;
    return true;
}

bool op_25() {
    cpu.R1 = cpu.R1;
    return true;
}

bool op_26() {
    cpu.R1 = cpu.R2;
    return true;
}

bool op_27() {
    cpu.R1 = cpu.R2;
    return true;
}

bool op_28() {
    cpu.R2 = cpu.R0;
    return true;
}

bool op_29() {
    cpu.R2 = cpu.R1;
    return true;
}

bool op_2a() {
    cpu.R2 = cpu.R2;
    return true;
}

bool op_2b() {
    cpu.R2 = cpu.R3;
    return true;
}

bool op_2c() {
    cpu.R3 = cpu.R0;
    return true;
}

bool op_2d() {
    cpu.R3 = cpu.R1;
    return true;
}

bool op_2e() {
    cpu.R3 = cpu.R2;
    return true;
}

bool op_2f() {
    cpu.R3 = cpu.R3;
    return true;
}

bool op_30() {
    cpu.R0 += cpu.R0;
    return true;
}

bool op_31() {
    cpu.R0 += cpu.R1;
    return true;
}

bool op_32() {
    cpu.R0 += cpu.R2;
    return true;
}

bool op_33() {
    cpu.R0 += cpu.R3;
    return true;
}

bool op_34() {
    cpu.R1 += cpu.R0;
    return true;
}

bool op_35() {
    cpu.R1 += cpu.R1;
    return true;
}

bool op_36() {
    cpu.R1 += cpu.R2;
    return true;
}

bool op_37() {
    cpu.R1 += cpu.R3;
    return true;
}

bool op_38() {
    cpu.R2 += cpu.R0;
    return true;
}

bool op_39() {
    cpu.R2 += cpu.R1;
    return true;
}

bool op_3a() {
    cpu.R2 += cpu.R2;
    return true;
}

bool op_3b() {
    cpu.R2 += cpu.R3;
    return true;
}

bool op_3c() {
    cpu.R3 += cpu.R0;
    return true;
}

bool op_3d() {
    cpu.R3 += cpu.R1;
    return true;
}

bool op_3e() {
    cpu.R3 += cpu.R2;
    return true;
}

bool op_3f() {
    cpu.R3 += cpu.R3;
    return true;
}

bool op_40() {
    cpu.R0 = mem_read(cpu.R0);
    return true;
}

bool op_41() {
    cpu.R0 = mem_read(cpu.R1);
    return true;
}

bool op_42() {
    cpu.R0 = mem_read(cpu.R2);
    return true;
}

bool op_43() {
    cpu.R0 = mem_read(cpu.R3);
    return true;
}

bool op_44() {
    cpu.R1 = mem_read(cpu.R0);
    return true;
}

bool op_45() {
    cpu.R1 = mem_read(cpu.R1);
    return true;
}

bool op_46() {
    cpu.R1 = mem_read(cpu.R2);
    return true;
}

bool op_47() {
    cpu.R1 = mem_read(cpu.R3);
    return true;
}

bool op_48() {
    cpu.R2 = mem_read(cpu.R0);
    return true;
}

bool op_49() {
    cpu.R2 = mem_read(cpu.R1);
    return true;
}

bool op_4a() {
    cpu.R2 = mem_read(cpu.R2);
    return true;
}

bool op_4b() {
    cpu.R2 = mem_read(cpu.R3);
    return true;
}

bool op_4c() {
    cpu.R3 = mem_read(cpu.R0);
    return true;
}

bool op_4d() {
    cpu.R3 = mem_read(cpu.R1);
    return true;
}

bool op_4e() {
    cpu.R3 = mem_read(cpu.R2);
    return true;
}

bool op_4f() {
    cpu.R3 = mem_read(cpu.R3);
    return true;
}

bool op_50() {
    cpu.R0 = mem_read_16(cpu.R0);
    return true;
}

bool op_51() {
    cpu.R0 = mem_read_16(cpu.R1);
    return true;
}

bool op_52() {
    cpu.R0 = mem_read_16(cpu.R2);
    return true;
}

bool op_53() {
    cpu.R0 = mem_read_16(cpu.R3);
    return true;
}

bool op_54() {
    cpu.R1 = mem_read_16(cpu.R0);
    return true;
}

bool op_55() {
    cpu.R1 = mem_read_16(cpu.R1);
    return true;
}

bool op_56() {
    cpu.R1 = mem_read_16(cpu.R2);
    return true;
}

bool op_57() {
    cpu.R1 = mem_read_16(cpu.R3);
    return true;
}

bool op_58() {
    cpu.R2 = mem_read_16(cpu.R0);
    return true;
}

bool op_59() {
    cpu.R2 = mem_read_16(cpu.R1);
    return true;
}

bool op_5a() {
    cpu.R2 = mem_read_16(cpu.R2);
    return true;
}

bool op_5b() {
    cpu.R2 = mem_read_16(cpu.R3);
    return true;
}

bool op_5c() {
    cpu.R3 = mem_read_16(cpu.R0);
    return true;
}

bool op_5d() {
    cpu.R3 = mem_read_16(cpu.R1);
    return true;
}

bool op_5e() {
    cpu.R3 = mem_read_16(cpu.R2);
    return true;
}

bool op_5f() {
    cpu.R3 = mem_read_16(cpu.R3);
    return true;
}

bool op_60() {
    mem_write(cpu.R0, cpu.R0);
    return true;
}

bool op_61() {
    mem_write(cpu.R0, cpu.R1);
    return true;
}

bool op_62() {
    mem_write(cpu.R0, cpu.R2);
    return true;
}

bool op_63() {
    mem_write(cpu.R0, cpu.R3);
    return true;
}

bool op_64() {
    mem_write(cpu.R1, cpu.R0);
    return true;
}

bool op_65() {
    mem_write(cpu.R1, cpu.R1);
    return true;
}

bool op_66() {
    mem_write(cpu.R1, cpu.R2);
    return true;
}

bool op_67() {
    mem_write(cpu.R1, cpu.R3);
    return true;
}

bool op_68() {
    mem_write(cpu.R2, cpu.R0);
    return true;
}

bool op_69() {
    mem_write(cpu.R2, cpu.R1);
    return true;
}

bool op_6a() {
    mem_write(cpu.R2, cpu.R2);
    return true;
}

bool op_6b() {
    mem_write(cpu.R2, cpu.R3);
    return true;
}

bool op_6c() {
    mem_write(cpu.R3, cpu.R0);
    return true;
}

bool op_6d() {
    mem_write(cpu.R3, cpu.R1);
    return true;
}

bool op_6e() {
    mem_write(cpu.R3, cpu.R2);
    return true;
}

bool op_6f() {
    mem_write(cpu.R3, cpu.R3);
    return true;
}

bool op_70() {
    mem_write_16(cpu.R0, cpu.R0);
    return true;
}

bool op_71() {
    mem_write_16(cpu.R0, cpu.R1);
    return true;
}

bool op_72() {
    mem_write_16(cpu.R0, cpu.R2);
    return true;
}

bool op_73() {
    mem_write_16(cpu.R0, cpu.R3);
    return true;
}

bool op_74() {
    mem_write_16(cpu.R1, cpu.R0);
    return true;
}

bool op_75() {
    mem_write_16(cpu.R1, cpu.R1);
    return true;
}

bool op_76() {
    mem_write_16(cpu.R1, cpu.R2);
    return true;
}

bool op_77() {
    mem_write_16(cpu.R1, cpu.R3);
    return true;
}

bool op_78() {
    mem_write_16(cpu.R2, cpu.R0);
    return true;
}

bool op_79() {
    mem_write_16(cpu.R2, cpu.R1);
    return true;
}

bool op_7a() {
    mem_write_16(cpu.R2, cpu.R2);
    return true;
}

bool op_7b() {
    mem_write_16(cpu.R2, cpu.R3);
    return true;
}

bool op_7c() {
    mem_write_16(cpu.R3, cpu.R0);
    return true;
}

bool op_7d() {
    mem_write_16(cpu.R3, cpu.R1);
    return true;
}

bool op_7e() {
    mem_write_16(cpu.R3, cpu.R2);
    return true;
}

bool op_7f() {
    mem_write_16(cpu.R3, cpu.R3);
    return true;
}

bool op_80() {
    cmp(cpu.R0, cpu.R0);
    return true;
}

bool op_81() {
    cmp(cpu.R0, cpu.R1);
    return true;
}

bool op_82() {
    cmp(cpu.R0, cpu.R2);
    return true;
}

bool op_83() {
    cmp(cpu.R0, cpu.R3);
    return true;
}

bool op_84() {
    cmp(cpu.R1, cpu.R0);
    return true;
}

bool op_85() {
    cmp(cpu.R1, cpu.R1);
    return true;
}

bool op_86() {
    cmp(cpu.R1, cpu.R2);
    return true;
}

bool op_87() {
    cmp(cpu.R1, cpu.R3);
    return true;
}

bool op_88() {
    cmp(cpu.R2, cpu.R0);
    return true;
}

bool op_89() {
    cmp(cpu.R2, cpu.R1);
    return true;
}

bool op_8a() {
    cmp(cpu.R2, cpu.R2);
    return true;
}

bool op_8b() {
    cmp(cpu.R2, cpu.R3);
    return true;
}

bool op_8c() {
    cmp(cpu.R3, cpu.R0);
    return true;
}

bool op_8d() {
    cmp(cpu.R3, cpu.R1);
    return true;
}

bool op_8e() {
    cmp(cpu.R3, cpu.R2);
    return true;
}

bool op_8f() {
    cmp(cpu.R3, cpu.R3);
    return true;
}

bool op_90() {
    push_16(cpu.R0);
    return true;
}

bool op_91() {
    push_16(cpu.R1);
    return true;
}

bool op_92() {
    push_16(cpu.R2);
    return true;
}

bool op_93() {
    push_16(cpu.R3);
    return true;
}

bool op_94() {
    cpu.R0 = pop_16();
    return true;
}

bool op_95() {
    cpu.R1 = pop_16();
    return true;
}

bool op_96() {
    cpu.R2 = pop_16();
    return true;
}

bool op_97() {
    cpu.R3 = pop_16();
    return true;
}

bool op_98() {
    uint16_t dest_addr = mem_read_16(cpu.PC);
    cpu.PC = dest_addr;
    return true;
}

bool op_99() {
    uint16_t dest_addr = read_op_imm();

    push_16(cpu.PC);

    cpu.PC = dest_addr;
    return true;
}

bool op_9a() {
    uint16_t value = read_op_imm();

    if (cpu.flag_l) cpu.PC = value;
    return true;
}

bool op_9b() {
    uint16_t value = read_op_imm();
    
    if (cpu.flag_g) cpu.PC = value;
    return true;
}

bool op_9c() {
    uint16_t value = read_op_imm();
    
    if (cpu.flag_z) cpu.PC = value;
    return true;
}

bool op_9d() {
    uint16_t value = read_op_imm();
    
    if (!cpu.flag_z) cpu.PC = value;
    return true;
}

bool op_9e() {
    uint16_t dest_addr = read_op_imm();

    if (cpu.flag_l) {
        push_16(cpu.PC);
        cpu.PC = dest_addr;
    }
    return true;
}

bool op_9f() {
    uint16_t dest_addr = read_op_imm();

    if (cpu.flag_g) {
        push_16(cpu.PC);
        cpu.PC = dest_addr;
    }
    return true;
}

bool op_a0() {
    uint16_t dest_addr = read_op_imm();

    if (cpu.flag_z) {
        push_16(cpu.PC);
        cpu.PC = dest_addr;
    }
    return true;
}

bool op_a1() {
    uint16_t dest_addr = read_op_imm();

    if (!cpu.flag_z) {
        push_16(cpu.PC);
        cpu.PC = dest_addr;
    }
    return true;
}

bool op_a2() {
    uint16_t value = read_op_imm();
    cmp(cpu.R0, value);
    return true;
}

bool op_a3() {
    uint16_t value = read_op_imm();
    cmp(cpu.R1, value);
    return true;
}

bool op_a4() {
    uint16_t value = read_op_imm();
    cmp(cpu.R2, value);
    return true;
}

bool op_a5() {
    uint16_t value = read_op_imm();
    cmp(cpu.R3, value);
    return true;
}

bool op_a6() {
    push_16(cpu.SP);
    return true;
}

bool op_a7() {
    push_16(cpu.PC - 1);
    return true;
}

bool op_a8() {
    cpu.R0++;
    return true;
}

bool op_a9() {
    cpu.R1++;
    return true;
}

bool op_aa() {
    cpu.R2++;
    return true;
}

bool op_ab() {
    cpu.R3++;
    return true;
}

bool op_ac() {
    cpu.R0--;
    return true;
}

bool op_ad() {
    cpu.R1--;
    return true;
}

bool op_ae() {
    cpu.R2--;
    return true;
}

bool op_af() {
    cpu.R3--;
    return true;
}

bool op_b0() {
    uint16_t address = read_op_imm();
    cpu.R0 = mem_read(address);
    return true;
}

bool op_b1() {
    uint16_t address = read_op_imm();
    cpu.R1 = mem_read(address);
    return true;
}

bool op_b2() {
    uint16_t address = read_op_imm();
    cpu.R2 = mem_read(address);
    return true;
}

bool op_b3() {
    uint16_t address = read_op_imm();
    cpu.R3 = mem_read(address);
    return true;
}

bool op_b4() {
    uint16_t address = read_op_imm();
    cpu.R0 = mem_read_16(address);
    return true;
}

bool op_b5() {
    uint16_t address = read_op_imm();
    cpu.R1 = mem_read_16(address);
    return true;
}

bool op_b6() {
    uint16_t address = read_op_imm();
    cpu.R2 = mem_read_16(address);
    return true;
}

bool op_b7() {
    uint16_t address = read_op_imm();
    cpu.R3 = mem_read_16(address);
    return true;
}

bool op_b8() {
    uint16_t address = read_op_imm();
    mem_write(address, cpu.R0 & 0xff);
    return true;
}

bool op_b9() {
    uint16_t address = read_op_imm();
    mem_write(address, cpu.R1 & 0xff);
    return true;
}

bool op_ba() {
    uint16_t address = read_op_imm();
    mem_write(address, cpu.R2 & 0xff);
    return true;
}

bool op_bb() {
    uint16_t address = read_op_imm();
    mem_write(address, cpu.R3 & 0xff);
    return true;
}

bool op_bc() {
    uint16_t address = read_op_imm();
    mem_write_16(address, cpu.R0);
    return true;
}

bool op_bd() {
    uint16_t address = read_op_imm();
    mem_write_16(address, cpu.R1);
    return true;
}

bool op_be() {
    uint16_t address = read_op_imm();
    mem_write_16(address, cpu.R2);
    return true;
}

bool op_bf() {
    uint16_t address = read_op_imm();
    mem_write_16(address, cpu.R3);
    return true;
}

bool op_c0() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_c1() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_c2() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_c3() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_c4() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_c5() {
    cpu.PC = cpu.R0;
    return true;
}

bool op_c6() {
    cpu.PC = cpu.R1;
    return true;
}

bool op_c7() {
    cpu.PC = cpu.R2;
    return true;
}

bool op_c8() {
    cpu.PC = cpu.R3;
    return true;
}

bool op_c9() {
    push_16(cpu.PC);
    cpu.PC = cpu.R0;
    return true;
}

bool op_ca() {
    push_16(cpu.PC);
    cpu.PC = cpu.R1;
    return true;
}

bool op_cb() {
    push_16(cpu.PC);
    cpu.PC = cpu.R2;
    return true;
}

bool op_cc() {
    push_16(cpu.PC);
    cpu.PC = cpu.R3;
    return true;
}

bool op_cd() {
    uint16_t value = read_op_imm();
    cpu.R0 = cpu.R0 & value;
    return true;
}

bool op_ce() {
    uint16_t value = read_op_imm();
    cpu.R0 = cpu.R0 | value;
    return true;
}

bool op_cf() {
uint16_t value = read_op_imm();
    cpu.R0 = cpu.R0 ^ value;
    return true;
}

bool op_d0() {
    cpu.R0 = cpu.R0 & cpu.R0;
    return true;
}

bool op_d1() {
    cpu.R0 = cpu.R0 & cpu.R1;
    return true;
}

bool op_d2() {
    cpu.R0 = cpu.R0 & cpu.R2;
    return true;
}

bool op_d3() {
    cpu.R0 = cpu.R0 & cpu.R3;
    return true;
}

bool op_d4() {
    cpu.R0 = cpu.R0 | cpu.R0;
    return true;
}

bool op_d5() {
    cpu.R0 = cpu.R0 | cpu.R1;
    return true;
}

bool op_d6() {
    cpu.R0 = cpu.R0 | cpu.R2;
    return true;
}

bool op_d7() {
    cpu.R0 = cpu.R0 | cpu.R3;
    return true;
}

bool op_d8() {
    cpu.R0 = cpu.R0 ^ cpu.R0;
    return true;
}

bool op_d9() {
    cpu.R0 = cpu.R0 ^ cpu.R1;
    return true;
}

bool op_da() {
    cpu.R0 = cpu.R0 ^ cpu.R2;
    return true;
}

bool op_db() {
    cpu.R0 = cpu.R0 ^ cpu.R3;
    return true;
}

bool op_dc() {
    cpu.R0 = (cpu.R0 >> 8) | (cpu.R0 << 8);
    return true;
}

bool op_dd() {
    cpu.R0 = (cpu.R0 >> 8) | (cpu.R0 << 8);
    return true;
}

bool op_de() {
    cpu.R0 = (cpu.R0 >> 8) | (cpu.R0 << 8);
    return true;
}

bool op_df() {
    cpu.R0 = (cpu.R0 >> 8) | (cpu.R0 << 8);
    return true;
}

bool op_e0() {
    uint16_t value = read_op_imm();
    cpu.R0 += value;
    return true;
}

bool op_e1() {
    uint16_t value = read_op_imm();
    cpu.R1 += value;
    return true;
}

bool op_e2() {
    uint16_t value = read_op_imm();
    cpu.R2 += value;
    return true;
}

bool op_e3() {
    uint16_t value = read_op_imm();
    cpu.R3 += value;
    return true;
}

bool op_e4() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_e5() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_e6() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_e7() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_e8() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_e9() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_ea() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_eb() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_ec() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_ed() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_ee() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_ef() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_f0() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_f1() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_f2() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_f3() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_f4() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_f5() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_f6() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_f7() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_f8() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_f9() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_fa() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_fb() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_fc() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_fd() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_fe() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}

bool op_ff() {
    printf("Illegal Instruction\n");
    exit(0);
    return false;
}
