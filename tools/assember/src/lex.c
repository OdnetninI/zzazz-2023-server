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
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define _LEX_H_IMP_
#include "lex.h"
#include "file.h"

uint32_t line = 1;
char putback = 0;
bool found_eof = false;

char next() {
    if (putback) {
        char c = putback;
        putback = 0;
        if (c == EOF) found_eof = true;
        return c;
    }
    
    char c = fgetc(input);
    if (c == '\n') line++;
    if (c == EOF) found_eof = true;
    return c;
}

void putBack(char c) {
    putback = c;
}

char skip() {
    char c = next();
    while (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f') {
        c = next();
    }
    if (c == EOF) found_eof = true;
    return c;
}

void skip_until_newline() {
    char c = next();
    while (c != '\n' && c!= EOF) {
        c = next();
    }
    if (c == EOF) found_eof = true;
    putBack(c);
}

bool isSpacer(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == ',' || c == EOF);
}

void identifyToken_a(Token* token) {
    switch(token->data[1]) {
        case 'd': token->type = T_I_add; break;
        case 'n': token->type = T_I_and; break;
    }
}

void identifyToken_call_(Token* token) {
    switch(token->data[5]) {
        case 'g': token->type = T_I_call_g; break;
        case 'l': token->type = T_I_call_l; break;
        case 'n': token->type = T_I_call_n; break;
        case 'z': token->type = T_I_call_z; break;
    }
}

void identifyToken_call(Token* token) {
    switch(token->data[4]) {
        case '\0': token->type = T_I_call; break;
        case '.': identifyToken_call_(token); break;
    }
}

void identifyToken_c(Token* token) {
    switch(token->data[1]) {
        case 'a': identifyToken_call(token); break;
        case 'm': token->type = T_I_cmp; break;
    }
}

void identifyToken_d(Token* token) {
    switch(token->data[1]) {
        case 'e': token->type = T_I_dec; break;
        case 'i': token->type = T_I_div; break;
    }
}

void identifyToken_i(Token* token) {
    switch(token->data[1]) {
        case 'l': token->type = T_I_illegal; break;
        case 'n': token->type = T_I_inc; break;
    }
}

void identifyToken_j_(Token* token) {
    switch(token->data[2]) {
        case 'g': token->type = T_I_j_g; break;
        case 'l': token->type = T_I_j_l; break;
        case 'z': token->type = T_I_j_z; break;
        case 'n': token->type = T_I_j_n; break;
    }
}

void identifyToken_j(Token* token) {
    switch(token->data[1]) {
        case '.': identifyToken_j_(token); break;
        case 'u': token->type = T_I_jump; break;
    }
}

void identifyToken_ld(Token* token) {
    switch(token->data[2]) {
        case '\0': token->type = T_I_ld; break;
        case 'b': token->type = T_I_ldb; break;
    }
}

void identifyToken_l(Token* token) {
    switch(token->data[1]) {
        case 'd': identifyToken_ld(token); break;
        case 'l': token->type = T_I_lls; break;
    }
}

void identifyToken_m(Token* token) {
    switch(token->data[1]) {
        case 'o': token->type = T_I_mov; break;
        case 'u': token->type = T_I_mul; break;
    }
}

void identifyToken_nop(Token* token) {
    switch(token->data[3]) {
        case '\0': token->type = T_I_nop; break;
        case 'c': token->type = T_I_nopc; break;
    }
}

void identifyToken_p(Token* token) {
    switch(token->data[1]) {
        case 'o': token->type = T_I_pop; break;
        case 'u': token->type = T_I_push; break;
    }
}

void identifyToken_r(Token* token) {
    switch(token->data[1]) {
        case 'e': token->type = T_I_ret; break;
        case 'l': token->type = T_I_rls; break;
    }
}

void identifyToken_st(Token* token) {
    switch(token->data[2]) {
        case '\0': token->type = T_I_st; break;
        case 'b': token->type = T_I_stb; break;
    }
}

void identifyToken_s(Token* token) {
    switch(token->data[1]) {
        case 't': identifyToken_st(token); break;
        case 'y': token->type = T_I_sys; break;
        case 'w': token->type = T_I_swap; break;
    }
}

void identifyToken(Token* token) {
    assert(token->data);
    token->type = T_Unknown;

    switch(token->data[0]) {
        case '.': token->type = T_TextID;   break;
        case '"': token->type = T_TextData; break;
        case '\'': token->type = T_Chars; break;
        case ':': token->type = T_Label;    break;
        case '$': token->type = T_Define;   break;
        case '#': token->type = T_Num;      break;
        case '%': token->type = T_Register; break;
        case '[': token->type = T_LSquareBracket; break;
        case ']': token->type = T_RSquareBracket; break;
        case '(': token->type = T_LParenthesis; break;
        case ')': token->type = T_RParenthesis; break;
        case '&': token->type = T_Implace; break;
        case '+': token->type = T_add; break;
        case '-': token->type = T_sub; break;

        case 'a': identifyToken_a(token); break;
        case 'b': token->type = T_I_break; break;
        case 'c': identifyToken_c(token); break;
        case 'd': identifyToken_d(token); break;
        case 'e': token->type = T_entry; break;

        case 'i': identifyToken_i(token); break;
        case 'j': identifyToken_j(token); break;
        case 'l': identifyToken_l(token); break;
        case 'm': identifyToken_m(token); break;
        case 'n': identifyToken_nop(token); break;

        case 'o': token->type = T_I_or; break;
        case 'p': identifyToken_p(token); break;

        case 'r': identifyToken_r(token); break;
        case 's': identifyToken_s(token); break;

        case 'x': token->type = T_I_xor; break;

        case 'z': token->type = T_I_zzazz; break;
    }
}

void nextToken(Token* token) {
    if (found_eof) {
        token->type = T_EOF;
        token->line = line;
        token->data = NULL;
        return;
    }

    char data[4096] = {0};
    char c = EOF;
    bool found_str = false;
    bool found_chars = false;
    bool found_implace = false;
    putBack(skip());
    int i;
    for(i = 0; ;++i) {
        c = next();   

        if (found_implace) {
            if (c == '&') {
                data[i] = c;
                data[i+1] = 0x00;
                break;
            }

            if (c == '\\') {
                c = next();
                if (c == 'n') c = '\n';
                if (c == '\\') c = '\\';
                if (c == '"') c = '"';
                if (c == '&') c = '&';
                if (c == 'x') {
                    char hex[3];
                    hex[0] = next();
                    hex[1] = next();
                    hex[2] = 0x00;
                    c = (char)strtol(hex, NULL, 16);
                }
            }

            data[i] = c;
            continue;
        }

        if (found_chars) {
            if (c == '\'') {
                data[i] = c;
                data[i+1] = 0x00;
                break;
            }

            if (c == '\\') {
                c = next();
                if (c == 'n') c = '\n';
                if (c == '\\') c = '\\';
                if (c == '"') c = '"';
                if (c == 'x') {
                    char hex[3];
                    hex[0] = next();
                    hex[1] = next();
                    hex[2] = 0x00;
                    c = (char)strtol(hex, NULL, 16);
                }
            }

            data[i] = c;
            continue;
        }    

        if (found_str) {
            if (c == '"') {
                data[i] = c;
                data[i+1] = 0x00;
                break;
            }

            if (c == '\\') {
                c = next();
                if (c == 'n') c = '\n';
                if (c == '\\') c = '\\';
                if (c == '"') c = '"';
                if (c == 'x') {
                    char hex[3];
                    hex[0] = next();
                    hex[1] = next();
                    hex[2] = 0x00;
                    c = (char)strtol(hex, NULL, 16);
                }
            }

            data[i] = c;
            continue;
        }

        if (c == EOF || found_eof) {
            token->type = T_EOF;
            token->line = line;
            token->data = NULL;
            return;
        }

        if (c == '&') {
            found_implace = true;
        }

        if (c == '\'') {
            found_chars = true;
        }

        if (c == '"') {
            found_str = true;
        }

        if (c == ';') {
            skip_until_newline();
            continue;
        }

        if (i == 0 && (c == '(' || c == ')')) {
            data[i] = c;
            data[i+1] = 0x00;
            break;
        }

        if (c == ')') {
            data[i] = 0x00;
            putBack(c);
            break;
        }

        if (i == 0 && (c == '[' || c == ']')) {
            data[i] = c;
            data[i+1] = 0x00;
            break;
        }

        if (c == ']') {
            data[i] = 0x00;
            putBack(c);
            break;
        }
        
        if (isSpacer(c)) {
            data[i] = 0x00;

            // If empty token, just restart again
            if (data[0] == 0x00) {
                i = -1;
                continue;
            }

            break;
        }

        data[i] = c;
    }

    token->line = line;
    token->data_size = i;
    token->data = malloc(i+1);
    token->a = NULL;
    token->op = NULL;
    token->b = NULL;

    for (int j = 0; j <= i; ++j) token->data[j] = data[j];
    identifyToken(token);

    if (token->type == T_LParenthesis) {
        token->type = T_expression;

        Token* a = malloc(sizeof(Token));
        Token* op = malloc(sizeof(Token));
        Token* b = malloc(sizeof(Token));
        nextToken(a);
        nextToken(op);
        nextToken(b);
        Token close_exp;
        nextToken(&close_exp);
        assert(close_exp.type == T_RParenthesis);

        token->a = a;
        token->op = op;
        token->b = b;

        printf("Found expression: %s %s %s\n", a->data, op->data, b->data);
    }

    if (token->type == T_Chars) {
        char* data = malloc(6);
        switch(i) {
            case 2: sprintf(data, "#%04x", token->data[1]); break;
            case 3: sprintf(data, "#%02x%02x", token->data[2], token->data[1]); break;
            default: assert(false);
        }
        token->type = T_Num;
        free(token->data);
        token->data = data;
        token->data_size = 6;
    }
}
