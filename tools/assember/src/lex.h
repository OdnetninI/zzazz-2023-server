#ifndef __LEX_H__
#define __LEX_H__

#pragma once

#define FOREACH_STATE(State)           \
    State(T_Unknown)                   \
    State(T_EOF)                       \
    State(T_TextID)                    \
    State(T_TextData)                  \
    State(T_Label)                     \
    State(T_Define)                    \
    State(T_Num)                       \
    State(T_Register)                  \
    State(T_entry)                     \
    State(T_LSquareBracket)            \
    State(T_RSquareBracket)            \
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
} Token;

void nextToken(Token* token);

#endif /* __LEX_H__ */