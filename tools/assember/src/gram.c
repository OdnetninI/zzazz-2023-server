#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "lex.h"
#include "gram.h"

#include "file.h"
#include "code.h"
#include "symbols.h"

uint16_t decode_hex(const char* data) {
    return strtol(&(data[1]), NULL, 16);
}

uint16_t decode_reg(const char* reg) {
    switch(reg[1]) {
        case 'p':
        case 'P': return PC;
        
        case 's':
        case 'S': return SP;

        case 'r':
        case 'R': 
            switch(reg[2]) {
                case '0': return R0;
                case '1': return R1;
                case '2': return R2;
                case '3': return R3;
            }
    }
    return R_Unknown;
}

void parse_entry(Token* main) {
    Token token;
    nextToken(&token);
    assert(token.type == T_Num);
    uint16_t ep = decode_hex(token.data);
    setEntryPoint(ep);
    //printf("Entry %04x\n", ep);
    free(token.data);
}

void parse_define(Token* main) {
    Token token;
    nextToken(&token);
    assert(token.type == T_Num);
    //uint16_t define = decode_hex(token.data);
    //printf("Define %s -> %04x\n", main->data, define);
    add_define(main->data, decode_hex(token.data));
    free(token.data);
}

void parse_text(Token* main) {
    Token token;
    nextToken(&token);
    assert(token.type == T_TextData);
    //printf("Text %s -> %.*s\n", main->data, token.data_size, token.data);
    add_text(main->data, token.data + 1, token.data_size - 1);
    free(token.data);
}

void parse_label(Token* main) {
    //printf("Label %s\n", main->data);
    add_label(main->data, calculate_current_pos_code());
}


void parse_0_arg(Token* main) {
    //printf("%s\n", main->data);
    Token token = {.type = T_Unknown};
    addInstruction(main->type, R_Unknown, R_Unknown, NULL, NULL, NULL, NULL, token);
}

void parse_arg(Token* main) {
    Token t_data;
    nextToken(&t_data);
    switch(t_data.type) {
        case T_Label:
            //printf("%s %s\n", main->data, t_data.data);
            addInstruction(main->type, R_Unknown, R_Unknown, t_data.data, NULL, NULL, NULL, t_data);
            break;
        case T_TextID:
            //printf("%s %s\n", main->data, t_data.data);
            addInstruction(main->type, R_Unknown, R_Unknown, NULL, t_data.data, NULL, NULL, t_data);
            break;
        case T_Define:
            //printf("%s %s\n", main->data, t_data.data);
            addInstruction(main->type, R_Unknown, R_Unknown, NULL, NULL, t_data.data, NULL, t_data);
            break;
        case T_Num:
            //uint16_t value = decode_hex(t_data.data);
            //printf("%s %04x\n", main->data, value);
            addInstruction(main->type, R_Unknown, R_Unknown, NULL, NULL, NULL, t_data.data, t_data);
            break;
        case T_Register:
            uint16_t reg = decode_reg(t_data.data);
            //printf("%s %d\n", main->data, reg);
            addInstruction(main->type, reg, R_Unknown, NULL, NULL, NULL, NULL, t_data);
            break;
        case T_expression:
            //printf("%s %s\n", main->data, t_data.data);
            addInstruction(main->type, R_Unknown, R_Unknown, NULL, NULL, NULL, NULL, t_data);
            break;
        default: assert(false);
    }
    free(t_data.data);
}

void parse_2_args(Token* main) {
    Token t_reg;
    nextToken(&t_reg);
    Token t_data;
    nextToken(&t_data);

    assert(t_reg.type == T_Register);
    uint16_t reg = decode_reg(t_reg.data);

    switch(t_data.type) {
        case T_Label:
            //printf("%s %d, LABEL %s\n", main->data, reg, t_data.data);
            addInstruction(main->type, reg, R_Unknown, t_data.data, NULL, NULL, NULL, t_data);
            break;
        case T_TextID:
            //printf("%s %d, TEXT %s\n", main->data, reg, t_data.data);
            addInstruction(main->type, reg, R_Unknown, NULL, t_data.data, NULL, NULL, t_data);
            break;
        case T_Define:
            //printf("%s %d, DEFINE %s\n", main->data, reg, t_data.data);
            addInstruction(main->type, reg, R_Unknown, NULL, NULL, t_data.data, NULL, t_data);
            break;
        case T_Num:
            //uint16_t value = decode_hex(t_data.data);
            //printf("%s %d, %04x\n", main->data, reg, value);
            addInstruction(main->type, reg, R_Unknown, NULL, NULL, NULL, t_data.data, t_data);
            break;
        case T_Register:
            uint16_t reg2 = decode_reg(t_data.data);
            //printf("%s %d, %d\n", main->data, reg, reg2);
            addInstruction(main->type, reg, reg2, NULL, NULL, NULL, NULL, t_data);
            break;
        case T_expression:
            //printf("%s %d, LABEL %s\n", main->data, reg, t_data.data);
            addInstruction(main->type, reg, R_Unknown, NULL, NULL, NULL, NULL, t_data);
            break;
        default: assert(false);
    }
    free(t_reg.data);
    free(t_data.data);
}

void parse_2_args_addr(Token* main) {
    Token t_reg;
    nextToken(&t_reg);
    
    Token t_tmp;
    nextToken(&t_tmp);
    assert(t_tmp.type == T_LSquareBracket);
    free(t_tmp.data);

    Token t_data;
    nextToken(&t_data);

    nextToken(&t_tmp);
    assert(t_tmp.type == T_RSquareBracket);
    free(t_tmp.data);

    assert(t_reg.type == T_Register);
    uint16_t reg = decode_reg(t_reg.data);

    switch(t_data.type) {
        case T_TextID:
            //printf("%s %d, [%s]\n", main->data, reg, t_data.data);
            addInstruction(main->type, reg, R_Unknown, t_data.data, NULL, NULL, NULL, t_data);
            break;
        case T_Label:
            //printf("%s %d, [%s]\n", main->data, reg, t_data.data);
            addInstruction(main->type, reg, R_Unknown, NULL, t_data.data, NULL, NULL, t_data);
            break;
        case T_Define:
            //printf("%s %d, [%s]\n", main->data, reg, t_data.data);
            addInstruction(main->type, reg, R_Unknown, NULL, NULL, t_data.data, NULL, t_data);
            break;
        case T_Num:
            //uint16_t value = decode_hex(t_data.data);
            //printf("%s %d, [%04x]\n", main->data, reg, value);
            addInstruction(main->type, reg, R_Unknown, NULL, NULL, NULL, t_data.data, t_data);
            break;
        case T_Register:
            uint16_t reg2 = decode_reg(t_data.data);
            //printf("%s %d, [%d]\n", main->data, reg, reg2);
            addInstruction(main->type, reg, reg2, NULL, NULL, NULL, NULL, t_data);
            break;
        case T_expression:
            //printf("%s %d, [%s]\n", main->data, reg, t_data.data);
            addInstruction(main->type, reg, R_Unknown, NULL, NULL, NULL, NULL, t_data);
            break;
        default: assert(false);
    }

    free(t_reg.data);
    free(t_data.data);
}

void parse() {
    while (true) {
        Token token;
        nextToken(&token);

        if (token.type == T_Unknown) {
            printf("Error at line %d\n", token.line);
        }
        assert(token.type != T_Unknown);

        switch(token.type) {
            case T_EOF: return;
            case T_entry:  parse_entry(&token); break;
            case T_Define: parse_define(&token); break;
            case T_TextID: parse_text(&token); break;
            case T_Label:  parse_label(&token); break;
            
            // Instructions
            case T_I_ret:
            case T_I_nop:
            case T_I_nopc:
            case T_I_zzazz:
            case T_I_break: parse_0_arg(&token); break;

            case T_I_lls:
            case T_I_rls:
            case T_I_inc:
            case T_I_dec:
            case T_I_sys:
            case T_I_pop:
            case T_I_push:
            case T_I_jump:
            case T_I_j_g:
            case T_I_j_l:
            case T_I_j_n:
            case T_I_j_z:
            case T_I_call:
            case T_I_call_g:
            case T_I_call_l:
            case T_I_call_n:
            case T_I_call_z: parse_arg(&token); break;

            case T_I_add:
            case T_I_and:
            case T_I_or:
            case T_I_xor:
            case T_I_mul:
            case T_I_div:
            case T_I_cmp:
            case T_I_mov: parse_2_args(&token); break;

            case T_I_ld:
            case T_I_ldb:
            case T_I_st:
            case T_I_stb: parse_2_args_addr(&token); break;

            default: assert(false);
        }
        free(token.data);
    }
}