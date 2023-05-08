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

#ifndef __LEX_H__
#define __LEX_H__

#pragma once

#define FOREACH_STATE(State)           \
    State(T_Unknown)                   \
    State(T_EOF)                       \
    State(T_TextID)                    \
    State(T_Chars)                    \
    State(T_TextData)                  \
    State(T_Label)                     \
    State(T_Define)                    \
    State(T_Num)                       \
    State(T_Register)                  \
    State(T_entry)                     \
    State(T_LSquareBracket)            \
    State(T_RSquareBracket)            \
    State(T_Implace)                   \
    \
    /* Expressions */ \
    State(T_LParenthesis)              \
    State(T_RParenthesis)              \
    State(T_expression)                \
    State(T_add)                       \
    State(T_sub)                       \
    \
    /* Instructions */ \
    State(T_I_add)                     \
    State(T_I_and)                     \
    \
    State(T_I_break)                   \
    \
    State(T_I_call)                    \
    State(T_I_call_g)                  \
    State(T_I_call_l)                  \
    State(T_I_call_n)                  \
    State(T_I_call_z)                  \
    State(T_I_cmp)                     \
    \
    State(T_I_dec)                     \
    State(T_I_div)                     \
    \
    State(T_I_illegal)                 \
    State(T_I_inc)                     \
    \
    State(T_I_j_g)                     \
    State(T_I_j_l)                     \
    State(T_I_j_z)                     \
    State(T_I_j_n)                     \
    State(T_I_jump)                    \
    \
    State(T_I_ld)                      \
    State(T_I_ldb)                     \
    State(T_I_lls)                     \
    \
    State(T_I_mov)                     \
    State(T_I_mul)                     \
    \
    State(T_I_nop)                     \
    State(T_I_nopc)                    \
    \
    State(T_I_or)                      \
    \
    State(T_I_pop)                     \
    State(T_I_push)                    \
    \
    State(T_I_ret)                     \
    State(T_I_rls)                     \
    \
    State(T_I_st)                      \
    State(T_I_stb)                     \
    State(T_I_sys)                     \
    State(T_I_swap)                    \
    \
    State(T_I_xor)                     \
    \
    State(T_I_zzazz)                   \



#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

enum Tokens {
    FOREACH_STATE(GENERATE_ENUM)
    Num_Tokens
};

#ifdef _LEX_H_IMP_
const char *Token_to_Str[Num_Tokens] = {
    FOREACH_STATE(GENERATE_STRING)
};
#else 
extern const char *Token_to_Str[Num_Tokens];
#endif

#undef FOREACH_STATE
#undef GENERATE_ENUM
#undef GENERATE_STRING

typedef struct Token {
    uint32_t type;
    char* data;
    uint32_t data_size;
    uint32_t line;

    // Only for expressions
    struct Token* a;
    struct Token* op;
    struct Token* b;
} Token;

void nextToken(Token* token);

#endif /* __LEX_H__ */