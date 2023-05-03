#ifndef __CODE_H__
#define __CODE_H__

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

typedef struct Instruction {
    int instruction;
    uint8_t opcode;
    uint8_t inst_size;
    uint16_t reg;
    uint16_t regm;
    char* label;
    char* text;
    char* define;
    char* imm;
    struct Instruction* next;
} Instruction;

void setEntryPoint(uint16_t ep);
void addInstruction(int inst, uint16_t reg, uint16_t regm, char* label, char* text, char* define, char* imm);
uint16_t calculate_current_pos_code();

void dump_code();

#endif /* __CODE_H__ */