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

bool op_00() {
    SP -= 2;
    mem_write_16(SP, pc);
    pc = 0xfff0;
    return true;
}

bool op_01() {
    uint16_t value = mem_read_16(pc);
    pc += 2;
    R0 = R0 * value;
    return true;
}

bool op_02() {
    R0 = R0 * R1;
    return true;
}

bool op_03() {
    uint16_t value = mem_read_16(pc);
    pc += 2;
    R0 = R0 / value;
    return true;
}

bool op_04() {
    R0 = R0 / R1;
    return true;
}

bool op_05() {
    uint16_t addr = mem_read_16(SP);
    SP += 2;
    pc = addr;
    return true;
}

bool op_06() {
    uint8_t syscall_id = mem_read(pc);
    pc++;
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
    SP = mem_read_16(pc);
    pc += 2;
    return true;
}

bool op_0a() {
    R0 = SP;
    return true;
}

bool op_0b() {
    R1 = SP;
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
    R0 = R0 << 1;
    return true;
}

bool op_0f() {
    R0 = R0 >> 1;
    return true;
}

bool op_10() {
    R0 = mem_read_16(pc);
    pc += 2;
    return true;
}

bool op_11() {
    R1 = mem_read_16(pc);
    pc += 2;
    return true;
}

bool op_12() {
    R2 = mem_read_16(pc);
    pc += 2;
    return true;
}

bool op_13() {
    R3 = mem_read_16(pc);
    pc += 2;
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
    R0 = R0;
    return true;
}

bool op_21() {
    R0 = R1;
    return true;
}

bool op_22() {
    R0 = R2;
    return true;
}

bool op_23() {
    R0 = R3;
    return true;
}

bool op_24() {
    R1 = R0;
    return true;
}

bool op_25() {
    R1 = R1;
    return true;
}

bool op_26() {
    R1 = R2;
    return true;
}

bool op_27() {
    R1 = R2;
    return true;
}

bool op_28() {
    R2 = R0;
    return true;
}

bool op_29() {
    R2 = R1;
    return true;
}

bool op_2a() {
    R2 = R2;
    return true;
}

bool op_2b() {
    R2 = R3;
    return true;
}

bool op_2c() {
    R3 = R0;
    return true;
}

bool op_2d() {
    R3 = R1;
    return true;
}

bool op_2e() {
    R3 = R2;
    return true;
}

bool op_2f() {
    R3 = R3;
    return true;
}

bool op_30() {
    R0 += R0;
    return true;
}

bool op_31() {
    R0 += R1;
    return true;
}

bool op_32() {
    R0 += R2;
    return true;
}

bool op_33() {
    R0 += R3;
    return true;
}

bool op_34() {
    R1 += R0;
    return true;
}

bool op_35() {
    R1 += R1;
    return true;
}

bool op_36() {
    R1 += R2;
    return true;
}

bool op_37() {
    R1 += R3;
    return true;
}

bool op_38() {
    R2 += R0;
    return true;
}

bool op_39() {
    R2 += R1;
    return true;
}

bool op_3a() {
    R2 += R2;
    return true;
}

bool op_3b() {
    R2 += R3;
    return true;
}

bool op_3c() {
    R3 += R0;
    return true;
}

bool op_3d() {
    R3 += R1;
    return true;
}

bool op_3e() {
    R3 += R2;
    return true;
}

bool op_3f() {
    R3 += R3;
    return true;
}

bool op_40() {
    R0 = mem_read(R0);
    return true;
}

bool op_41() {
    R0 = mem_read(R1);
    return true;
}

bool op_42() {
    R0 = mem_read(R2);
    return true;
}

bool op_43() {
    R0 = mem_read(R3);
    return true;
}

bool op_44() {
    R1 = mem_read(R0);
    return true;
}

bool op_45() {
    R1 = mem_read(R1);
    return true;
}

bool op_46() {
    R1 = mem_read(R2);
    return true;
}

bool op_47() {
    R1 = mem_read(R3);
    return true;
}

bool op_48() {
    R2 = mem_read(R0);
    return true;
}

bool op_49() {
    R2 = mem_read(R1);
    return true;
}

bool op_4a() {
    R2 = mem_read(R2);
    return true;
}

bool op_4b() {
    R2 = mem_read(R3);
    return true;
}

bool op_4c() {
    R3 = mem_read(R0);
    return true;
}

bool op_4d() {
    R3 = mem_read(R1);
    return true;
}

bool op_4e() {
    R3 = mem_read(R2);
    return true;
}

bool op_4f() {
    R3 = mem_read(R3);
    return true;
}

bool op_50() {
    R0 = mem_read_16(R0);
    return true;
}

bool op_51() {
    R0 = mem_read_16(R1);
    return true;
}

bool op_52() {
    R0 = mem_read_16(R2);
    return true;
}

bool op_53() {
    R0 = mem_read_16(R3);
    return true;
}

bool op_54() {
    R1 = mem_read_16(R0);
    return true;
}

bool op_55() {
    R1 = mem_read_16(R1);
    return true;
}

bool op_56() {
    R1 = mem_read_16(R2);
    return true;
}

bool op_57() {
    R1 = mem_read_16(R3);
    return true;
}

bool op_58() {
    R2 = mem_read_16(R0);
    return true;
}

bool op_59() {
    R2 = mem_read_16(R1);
    return true;
}

bool op_5a() {
    R2 = mem_read_16(R2);
    return true;
}

bool op_5b() {
    R2 = mem_read_16(R3);
    return true;
}

bool op_5c() {
    R3 = mem_read_16(R0);
    return true;
}

bool op_5d() {
    R3 = mem_read_16(R1);
    return true;
}

bool op_5e() {
    R3 = mem_read_16(R2);
    return true;
}

bool op_5f() {
    R3 = mem_read_16(R3);
    return true;
}

bool op_60() {
    mem_write(R0, R0);
    return true;
}

bool op_61() {
    mem_write(R0, R1);
    return true;
}

bool op_62() {
    mem_write(R0, R2);
    return true;
}

bool op_63() {
    mem_write(R0, R3);
    return true;
}

bool op_64() {
    mem_write(R1, R0);
    return true;
}

bool op_65() {
    mem_write(R1, R1);
    return true;
}

bool op_66() {
    mem_write(R1, R2);
    return true;
}

bool op_67() {
    mem_write(R1, R3);
    return true;
}

bool op_68() {
    mem_write(R2, R0);
    return true;
}

bool op_69() {
    mem_write(R2, R1);
    return true;
}

bool op_6a() {
    mem_write(R2, R2);
    return true;
}

bool op_6b() {
    mem_write(R2, R3);
    return true;
}

bool op_6c() {
    mem_write(R3, R0);
    return true;
}

bool op_6d() {
    mem_write(R3, R1);
    return true;
}

bool op_6e() {
    mem_write(R3, R2);
    return true;
}

bool op_6f() {
    mem_write(R3, R3);
    return true;
}

bool op_70() {
    mem_write_16(R0, R0);
    return true;
}

bool op_71() {
    mem_write_16(R0, R1);
    return true;
}

bool op_72() {
    mem_write_16(R0, R2);
    return true;
}

bool op_73() {
    mem_write_16(R0, R3);
    return true;
}

bool op_74() {
    mem_write_16(R1, R0);
    return true;
}

bool op_75() {
    mem_write_16(R1, R1);
    return true;
}

bool op_76() {
    mem_write_16(R1, R2);
    return true;
}

bool op_77() {
    mem_write_16(R1, R3);
    return true;
}

bool op_78() {
    mem_write_16(R2, R0);
    return true;
}

bool op_79() {
    mem_write_16(R2, R1);
    return true;
}

bool op_7a() {
    mem_write_16(R2, R2);
    return true;
}

bool op_7b() {
    mem_write_16(R2, R3);
    return true;
}

bool op_7c() {
    mem_write_16(R3, R0);
    return true;
}

bool op_7d() {
    mem_write_16(R3, R1);
    return true;
}

bool op_7e() {
    mem_write_16(R3, R2);
    return true;
}

bool op_7f() {
    mem_write_16(R3, R3);
    return true;
}

bool op_80() {
    flag_z = false;
    flag_l = false;
    flag_g = false;
    if (R0 == R0) flag_z = true;
    if (R0 < R0) flag_l = true;
    if (R0 > R0) flag_g = true;
    return true;
}

bool op_81() {
    flag_z = false;
    flag_l = false;
    flag_g = false;
    if (R0 == R1) flag_z = true;
    if (R0 < R1) flag_l = true;
    if (R0 > R1) flag_g = true;
    return true;
}

bool op_82() {
    flag_z = false;
    flag_l = false;
    flag_g = false;
    if (R0 == R2) flag_z = true;
    if (R0 < R2) flag_l = true;
    if (R0 > R2) flag_g = true;
    return true;
}

bool op_83() {
    flag_z = false;
    flag_l = false;
    flag_g = false;
    if (R0 == R3) flag_z = true;
    if (R0 < R3) flag_l = true;
    if (R0 > R3) flag_g = true;
    return true;
}

bool op_84() {
    flag_z = false;
    flag_l = false;
    flag_g = false;
    if (R1 == R0) flag_z = true;
    if (R1 < R0) flag_l = true;
    if (R1 > R0) flag_g = true;
    return true;
}

bool op_85() {
    flag_z = false;
    flag_l = false;
    flag_g = false;
    if (R1 == R1) flag_z = true;
    if (R1 < R1) flag_l = true;
    if (R1 > R1) flag_g = true;
    return true;
}

bool op_86() {
    flag_z = false;
    flag_l = false;
    flag_g = false;
    if (R1 == R2) flag_z = true;
    if (R1 < R2) flag_l = true;
    if (R1 > R2) flag_g = true;
    return true;
}

bool op_87() {
    flag_z = false;
    flag_l = false;
    flag_g = false;
    if (R1 == R3) flag_z = true;
    if (R1 < R3) flag_l = true;
    if (R1 > R3) flag_g = true;
    return true;
}

bool op_88() {
    flag_z = false;
    flag_l = false;
    flag_g = false;
    if (R2 == R0) flag_z = true;
    if (R2 < R0) flag_l = true;
    if (R2 > R0) flag_g = true;
    return true;
}

bool op_89() {
    flag_z = false;
    flag_l = false;
    flag_g = false;
    if (R2 == R1) flag_z = true;
    if (R2 < R1) flag_l = true;
    if (R2 > R1) flag_g = true;
    return true;
}

bool op_8a() {
    flag_z = false;
    flag_l = false;
    flag_g = false;
    if (R2 == R2) flag_z = true;
    if (R2 < R2) flag_l = true;
    if (R2 > R2) flag_g = true;
    return true;
}

bool op_8b() {
    flag_z = false;
    flag_l = false;
    flag_g = false;
    if (R2 == R3) flag_z = true;
    if (R2 < R3) flag_l = true;
    if (R2 > R3) flag_g = true;
    return true;
}

bool op_8c() {
    flag_z = false;
    flag_l = false;
    flag_g = false;
    if (R3 == R0) flag_z = true;
    if (R3 < R0) flag_l = true;
    if (R3 > R0) flag_g = true;
    return true;
}

bool op_8d() {
    flag_z = false;
    flag_l = false;
    flag_g = false;
    if (R3 == R1) flag_z = true;
    if (R3 < R1) flag_l = true;
    if (R3 > R1) flag_g = true;
    return true;
}

bool op_8e() {
    flag_z = false;
    flag_l = false;
    flag_g = false;
    if (R3 == R2) flag_z = true;
    if (R3 < R2) flag_l = true;
    if (R3 > R2) flag_g = true;
    return true;
}

bool op_8f() {
    flag_z = false;
    flag_l = false;
    flag_g = false;
    if (R3 == R3) flag_z = true;
    if (R3 < R3) flag_l = true;
    if (R3 > R3) flag_g = true;
    return true;
}

bool op_90() {
    SP -= 2;
    mem_write_16(SP, R0);
    return true;
}

bool op_91() {
    SP -= 2;
    mem_write_16(SP, R1);
    return true;
}

bool op_92() {
    SP -= 2;
    mem_write_16(SP, R2);
    return true;
}

bool op_93() {
    SP -= 2;
    mem_write_16(SP, R3);
    return true;
}

bool op_94() {
    R0 = mem_read_16(SP);
    SP += 2;
    return true;
}

bool op_95() {
    R1 = mem_read_16(SP);
    SP += 2;
    return true;
}

bool op_96() {
    R2 = mem_read_16(SP);
    SP += 2;
    return true;
}

bool op_97() {
    R3 = mem_read_16(SP);
    SP += 2;
    return true;
}

bool op_98() {
    uint16_t dest_addr = mem_read_16(pc);
    pc = dest_addr;
    return true;
}

bool op_99() {
    uint16_t dest_addr = mem_read_16(pc);
    pc += 2;

    SP -= 2;
    mem_write_16(SP, pc);

    pc = dest_addr;
    return true;
}

bool op_9a() {
    uint16_t value = mem_read_16(pc);
    pc += 2;

    if (flag_l) pc = value;
    return true;
}

bool op_9b() {
    uint16_t value = mem_read_16(pc);
    pc += 2;
    
    if (flag_g) pc = value;
    return true;
}

bool op_9c() {
    uint16_t value = mem_read_16(pc);
    pc += 2;
    
    if (flag_z) pc = value;
    return true;
}

bool op_9d() {
    uint16_t value = mem_read_16(pc);
    pc += 2;
    
    if (!flag_z) pc = value;
    return true;
}

bool op_9e() {
    uint16_t dest_addr = mem_read_16(pc);
    pc += 2;

    if (flag_l) {
        SP -= 2;
        mem_write_16(SP, pc);        

        pc = dest_addr;
    }
    return true;
}

bool op_9f() {
    uint16_t dest_addr = mem_read_16(pc);
    pc += 2;

    if (flag_g) {
        SP -= 2;
        mem_write_16(SP, pc);

        pc = dest_addr;
    }
    return true;
}

bool op_a0() {
    uint16_t dest_addr = mem_read_16(pc);
    pc += 2;

    if (flag_z) {
        SP -= 2;
        mem_write_16(SP, pc);

        pc = dest_addr;
    }
    return true;
}

bool op_a1() {
    uint16_t dest_addr = mem_read_16(pc);
    pc += 2;

    if (!flag_z) {
        SP -= 2;
        mem_write_16(SP, pc);

        pc = dest_addr;
    }
    return true;
}

bool op_a2() {
    uint16_t value = mem_read_16(pc);
    pc += 2;

    flag_z = false;
    flag_l = false;
    flag_g = false;
    if (R0 == value) flag_z = true;
    if (R0 < value) flag_l = true;
    if (R0 > value) flag_g = true;
    return true;
}

bool op_a3() {
    uint16_t value = mem_read_16(pc);
    pc += 2;

    flag_z = false;
    flag_l = false;
    flag_g = false;
    if (R1 == value) flag_z = true;
    if (R1 < value) flag_l = true;
    if (R1 > value) flag_g = true;
    return true;
}

bool op_a4() {
    uint16_t value = mem_read_16(pc);
    pc += 2;

    flag_z = false;
    flag_l = false;
    flag_g = false;
    if (R2 == value) flag_z = true;
    if (R2 < value) flag_l = true;
    if (R2 > value) flag_g = true;
    return true;
}

bool op_a5() {
    uint16_t value = mem_read_16(pc);
    pc += 2;

    flag_z = false;
    flag_l = false;
    flag_g = false;
    if (R3 == value) flag_z = true;
    if (R3 < value) flag_l = true;
    if (R3 > value) flag_g = true;
    return true;
}

bool op_a6() {
    SP -= 2;
    mem_write_16(SP, SP);
    return true;
}

bool op_a7() {
    SP -= 2;
    mem_write_16(SP, pc - 1);
    return true;
}

bool op_a8() {
    R0++;
    return true;
}

bool op_a9() {
    R1++;
    return true;
}

bool op_aa() {
    R2++;
    return true;
}

bool op_ab() {
    R3++;
    return true;
}

bool op_ac() {
    R0--;
    return true;
}

bool op_ad() {
    R1--;
    return true;
}

bool op_ae() {
    R2--;
    return true;
}

bool op_af() {
    R3--;
    return true;
}

bool op_b0() {
    uint16_t address = mem_read_16(pc);
    pc += 2;
    R0 = mem_read(address);
    return true;
}

bool op_b1() {
    uint16_t address = mem_read_16(pc);
    pc += 2;
    R1 = mem_read(address);
    return true;
}

bool op_b2() {
    uint16_t address = mem_read_16(pc);
    pc += 2;
    R2 = mem_read(address);
    return true;
}

bool op_b3() {
    uint16_t address = mem_read_16(pc);
    pc += 2;
    R3 = mem_read(address);
    return true;
}

bool op_b4() {
    uint16_t address = mem_read_16(pc);
    pc += 2;
    R0 = mem_read_16(address);
    return true;
}

bool op_b5() {
    uint16_t address = mem_read_16(pc);
    pc += 2;
    R1 = mem_read_16(address);
    return true;
}

bool op_b6() {
    uint16_t address = mem_read_16(pc);
    pc += 2;
    R2 = mem_read_16(address);
    return true;
}

bool op_b7() {
    uint16_t address = mem_read_16(pc);
    pc += 2;
    R3 = mem_read_16(address);
    return true;
}

bool op_b8() {
    uint16_t address = mem_read_16(pc);
    pc += 2;
    mem_write(address, R0 & 0xff);
    return true;
}

bool op_b9() {
    uint16_t address = mem_read_16(pc);
    pc += 2;
    mem_write(address, R1 & 0xff);
    return true;
}

bool op_ba() {
    uint16_t address = mem_read_16(pc);
    pc += 2;
    mem_write(address, R2 & 0xff);
    return true;
}

bool op_bb() {
    uint16_t address = mem_read_16(pc);
    pc += 2;
    mem_write(address, R3 & 0xff);
    return true;
}

bool op_bc() {
    uint16_t address = mem_read_16(pc);
    pc += 2;
    mem_write_16(address, R0);
    return true;
}

bool op_bd() {
    uint16_t address = mem_read_16(pc);
    pc += 2;
    mem_write_16(address, R1);
    return true;
}

bool op_be() {
    uint16_t address = mem_read_16(pc);
    pc += 2;
    mem_write_16(address, R2);
    return true;
}

bool op_bf() {
    uint16_t address = mem_read_16(pc);
    pc += 2;
    mem_write_16(address, R3);
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
    pc = R0;
    return true;
}

bool op_c6() {
    pc = R1;
    return true;
}

bool op_c7() {
    pc = R2;
    return true;
}

bool op_c8() {
    pc = R3;
    return true;
}

bool op_c9() {
    SP -= 2;
    mem_write_16(SP, pc);
    pc = R0;
    return true;
}

bool op_ca() {
    SP -= 2;
    mem_write_16(SP, pc);
    pc = R1;
    return true;
}

bool op_cb() {
    SP -= 2;
    mem_write_16(SP, pc);
    pc = R2;
    return true;
}

bool op_cc() {
    SP -= 2;
    mem_write_16(SP, pc);
    pc = R3;
    return true;
}

bool op_cd() {
    uint16_t value = mem_read_16(pc);
    pc += 2;
    R0 = R0 & value;
    return true;
}

bool op_ce() {
    uint16_t value = mem_read_16(pc);
    pc += 2;
    R0 = R0 | value;
    return true;
}

bool op_cf() {
uint16_t value = mem_read_16(pc);
    pc += 2;
    R0 = R0 ^ value;
    return true;
}

bool op_d0() {
    R0 = R0 & R0;
    return true;
}

bool op_d1() {
    R0 = R0 & R1;
    return true;
}

bool op_d2() {
    R0 = R0 & R2;
    return true;
}

bool op_d3() {
    R0 = R0 & R3;
    return true;
}

bool op_d4() {
    R0 = R0 | R0;
    return true;
}

bool op_d5() {
    R0 = R0 | R1;
    return true;
}

bool op_d6() {
    R0 = R0 | R2;
    return true;
}

bool op_d7() {
    R0 = R0 | R3;
    return true;
}

bool op_d8() {
    R0 = R0 ^ R0;
    return true;
}

bool op_d9() {
    R0 = R0 ^ R1;
    return true;
}

bool op_da() {
    R0 = R0 ^ R2;
    return true;
}

bool op_db() {
    R0 = R0 ^ R3;
    return true;
}

bool op_dc() {
    R0 = (R0 >> 8) | (R0 << 8);
    return true;
}

bool op_dd() {
    R0 = (R0 >> 8) | (R0 << 8);
    return true;
}

bool op_de() {
    R0 = (R0 >> 8) | (R0 << 8);
    return true;
}

bool op_df() {
    R0 = (R0 >> 8) | (R0 << 8);
    return true;
}

bool op_e0() {
    uint16_t value = mem_read_16(pc);
    pc += 2;
    R0 += value;
    return true;
}

bool op_e1() {
    uint16_t value = mem_read_16(pc);
    pc += 2;
    R1 += value;
    return true;
}

bool op_e2() {
    uint16_t value = mem_read_16(pc);
    pc += 2;
    R2 += value;
    return true;
}

bool op_e3() {
    uint16_t value = mem_read_16(pc);
    pc += 2;
    R3 += value;
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
