#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

FILE* input;
FILE* output;

uint32_t line = 0;
char putback = 0;

uint16_t entry_point = 0x0000;
char current_id[256];
char current_text[2048];

uint16_t eoc;

struct identifiers {
    char* name;
    char* text;
    int size;
    uint16_t addr;
    struct identifiers* next;
};

struct identifiers identifiers = { .name = NULL, .text = NULL, .next = NULL };

void add_id(int t_size) {
    struct identifiers* node = &identifiers;
    while(node->name != NULL) node = node->next;
    
    int size = strlen(current_id);
    node->name = malloc(size);
    node->text = malloc(t_size);
    node->size = t_size;
    node->next = malloc(sizeof(struct identifiers));
    node->addr = 0x0000;

    strcpy(node->name, current_id);
    for(int i = 0; i < t_size; ++i) node->text[i] = current_text[i];

    node->next->name = NULL;
}

struct identifiers* find_id(const char* id) {
    struct identifiers* node = &identifiers;
    while(node->name != NULL && strcmp(node->name, id) != 0) { node = node->next; }
    return node;
}

void reserve_id_space() {
    struct identifiers* node = &identifiers;
    while(node->name != NULL) {
        node->addr = eoc;
        int size = node->size;
        printf("Reserved %04x to identifier %s (size %04x)\n", eoc, node->name, size);
        eoc += size+1;
        node = node->next;
    }
}

void dump_identifiers() {
    struct identifiers* node = &identifiers;
    while(node->name != NULL) {
        printf("Dumping identifier %s\n", node->name);
        int size = node->size;
        node->text[size] = 0x00;
        fwrite(node->text, 1, size+1, output);
        node = node->next;
    }
}

enum inst_type {
    I_unknown = 0,
    I_movi,
    I_mov,
    I_movl,
    I_movt,
    I_movv,
    I_ld,
    I_ldi,
    I_ldb,
    I_ldbi,
    I_st,
    I_stb,
    I_stbi,
    I_inc,
    I_dec,
    I_call,
    I_callr,
    I_call_v,
    I_sys,
    I_sys_v,
    I_jump,
    I_jump_t,
    I_break,
    I_push,
    I_pop,
    I_sti,
    I_addi,
    I_cmp,
    I_cmpi,
    I_jz,
    I_jz_t,
    I_jnz,
    I_jnz_t,
    I_jlt,
    I_jgt,
    I_call_z,
    I_call_nz,
    I_call_l,
    I_call_g,
    I_ret,
    
    I_end,
};

struct inst {
    int type;
    int reg;
    int reg2;
    uint16_t imm;
    char* id;
    char* label;
    char* var;
    uint16_t size;
};

struct inst inst[0xffff];
int curr_inst = 0;

void add_inst(int type, int reg, int reg2, uint16_t imm, char* id, char* label, char* var, uint16_t size) {
    inst[curr_inst].type = type;
    inst[curr_inst].reg = reg;
    inst[curr_inst].reg2 = reg2;
    inst[curr_inst].imm = imm;
    inst[curr_inst].id = NULL;
    if (id) inst[curr_inst].id = strdup(id);
    inst[curr_inst].label = NULL;
    if (label) inst[curr_inst].label = strdup(label);
    inst[curr_inst].var = NULL;
    if (var) inst[curr_inst].var = strdup(var);
    inst[curr_inst].size = size;
    
    curr_inst++;
    inst[curr_inst].type = I_end;
}

void calculate_end_of_code() {
    eoc = entry_point;
    for (int i = 0; i <= curr_inst; ++i) {
        if (inst[i].type == I_end) return;
        eoc += inst[i].size;
    }
}

struct labels {
    char* name;
    uint16_t addr;
    struct labels* next;
};
struct labels labels = { .name = NULL, .addr = 0x0000, .next = NULL };

void add_label() {
    struct labels* node = &labels;
    while(node->name != NULL) node = node->next;
    
    node->name = strdup(current_id);

    calculate_end_of_code();
    node->addr = eoc;
    node->next = malloc(sizeof(struct labels));
    node->next->name = NULL;

    printf("Label %s found at position %04x\n", node->name, node->addr);
}

struct labels* find_label(const char* id) {
    struct labels* node = &labels;
    while(node->name != NULL && strcmp(node->name, id) != 0) { node = node->next; }
    return node;
}

struct vars {
    char* name;
    uint16_t addr;
    struct vars* next;
};
struct vars vars = { .name = NULL, .addr = 0x0000, .next = NULL };

void add_var(uint16_t addr) {
    struct vars* node = &vars;
    while(node->name != NULL) node = node->next;
    
    node->name = strdup(current_id);
    node->addr = addr;
    node->next = malloc(sizeof(struct vars));
    node->next->name = NULL;

    printf("Variable %s found pointing to %04x\n", node->name, node->addr);
}

struct vars* find_var(const char* id) {
    struct vars* node = &vars;
    while(node->name != NULL && strcmp(node->name, id) != 0) {
        node = node->next;
    }
    return node;
}

char next() {
    if (putback) {
        char c = putback;
        putback = 0;
        return c;
    }
    
    char c = fgetc(input);
    if (c == '\n') line++;
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
    return c;
}

enum registers {
    R0 = 0,
    R1,
    R2,
    R3,
    SP,
    PC,
};

enum Tokens {
    T_unknown,
    T_eof,
    T_id,
    T_label,
    T_var,
    T_entry,
    T_movi,
    T_mov,
    T_movl,
    T_movt,
    T_movv,
    T_ld,
    T_ldi,
    T_ldb,
    T_ldbi,
    T_st,
    T_stb,
    T_stbi,
    T_inc,
    T_dec,
    T_call,
    T_callr,
    T_call_v,
    T_sys,
    T_sys_v,
    T_jump,
    T_jump_t,
    T_break,
    T_push,
    T_pop,
    T_sti,
    T_addi,
    T_cmp,
    T_cmpi,
    T_jz,
    T_jz_t,
    T_jnz,
    T_jnz_t,
    T_jlt,
    T_jgt,
    T_call_z,
    T_call_nz,
    T_call_l,
    T_call_g,
    T_ret,
};

int match() {
    putBack(skip());
    char id[256] = {0};
    int i = 0;

    while(true) {
        char c = next();
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == ',' || c == EOF) {
            id[i] = 0x00;

            if (id[0] == '.') {
    for(int j = 0; j <= i; ++j) current_id[j] = id[j];
    return T_id;
            }

            if (id[0] == ':') {
    for(int j = 0; j <= i; ++j) current_id[j] = id[j];
    return T_label;
            }

            if (id[0] == '$') {
    for(int j = 0; j <= i; ++j) current_id[j] = id[j];
    return T_var;
            }
            
            if (strcmp("entry", id) == 0) { return T_entry; }
            if (strcmp("mov", id) == 0) { return T_mov; }
            if (strcmp("movi", id) == 0) { return T_movi; }
            if (strcmp("movl", id) == 0) { return T_movl; }
            if (strcmp("movt", id) == 0) { return T_movt; }
            if (strcmp("movv", id) == 0) { return T_movv; }
            if (strcmp("stb", id) == 0) { return T_stb; }
            if (strcmp("stbi", id) == 0) { return T_stbi; }
            if (strcmp("st", id) == 0) { return T_st; }
            if (strcmp("inc", id) == 0) { return T_inc; }
            if (strcmp("dec", id) == 0) { return T_dec; }
            if (strcmp("call", id) == 0) { return T_call; }
            if (strcmp("callr", id) == 0) { return T_callr; }
            if (strcmp("call_v", id) == 0) { return T_call_v; }
            if (strcmp("break", id) == 0) { return T_break; }
            if (strcmp("sys", id) == 0) { return T_sys; }
            if (strcmp("sys_v", id) == 0) { return T_sys_v; }
            if (strcmp("jump", id) == 0) { return T_jump; }
            if (strcmp("jump_t", id) == 0) { return T_jump_t; }
            if (strcmp("push", id) == 0) { return T_push; }
            if (strcmp("pop", id) == 0) { return T_pop; }
            if (strcmp("sti", id) == 0) { return T_sti; }
            if (strcmp("addi", id) == 0) { return T_addi; }
            if (strcmp("ldi", id) == 0) { return T_ldi; }
            if (strcmp("ld", id) == 0) { return T_ld; }
            if (strcmp("ldb", id) == 0) { return T_ldb; }
            if (strcmp("ldbi", id) == 0) { return T_ldbi; }
            if (strcmp("cmp", id) == 0) { return T_cmp; }
            if (strcmp("cmpi", id) == 0) { return T_cmpi; }
            if (strcmp("jz", id) == 0) { return T_jz; }
            if (strcmp("jz_t", id) == 0) { return T_jz_t; }
            if (strcmp("jnz", id) == 0) { return T_jnz; }
            if (strcmp("jnz_t", id) == 0) { return T_jnz_t; }
            if (strcmp("jgt", id) == 0) { return T_jgt; }
            if (strcmp("jlt", id) == 0) { return T_jlt; }
            if (strcmp("call.z", id) == 0) { return T_call_z; }
            if (strcmp("call.nz", id) == 0) { return T_call_nz; }
            if (strcmp("call.g", id) == 0) { return T_call_g; }
            if (strcmp("call.l", id) == 0) { return T_call_l; }
            if (strcmp("ret", id) == 0) { return T_ret; }

            if (c == EOF) return T_eof;

            printf("Readed %s\n", id);
            break;
        }
        id[i] = c;
        i++;
    }
    return T_unknown;
}

uint16_t match_hex() {
    putBack(skip());
    char id[256] = {0};
    int i = 0;

    while(true) {
        char c = next();
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f') {
            id[i] = 0x00;
            return strtol(id, NULL, 16);
        }
        id[i] = c;
        i++;
    }
}

int match_text() {
    putBack(skip());
    char id[2048] = {0};
    int i = 0;
    bool found_start = false;
    
    while(true) {
        char c = next();
        if (c == '"') {
            if (!found_start) {
    found_start = true;
    continue;
            }
            id[i] = 0x00;
            for(int j = 0; j <= i; ++j) current_text[j] = id[j];
            break;
        }
        if (c == '\\') {
            c = next();
            if (c == 'x') {
    char hex[3];
                hex[0] = next();
    hex[1] = next();
    hex[2] = 0x00;
    c = (char)strtol(hex, NULL, 16);
            }
            if (c == 'n') {
    c = '\n';
            }
            if (c == '"') {
    c = '"';
            }
        }
        id[i] = c;
        i++;
    }
    return i;
}

int match_reg() {
    putBack(skip());
    char id[256] = {0};
    int i = 0;
    
    while(true) {
        char c = next();
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == ',') {
            id[i] = 0x00;      
            if (strcmp("r0", id) == 0) { return R0; }
            if (strcmp("r1", id) == 0) { return R1; }
            if (strcmp("r2", id) == 0) { return R2; }
            if (strcmp("r3", id) == 0) { return R3; }
            if (strcmp("sp", id) == 0) { return SP; }
            if (strcmp("pc", id) == 0) { return PC; }
            printf("Readed %s\n", id);
            break;
        }
        id[i] = c;
        i++;
    }
    return 99;
}

int match_reg_addr() {
    putBack(skip());
    char id[256] = {0};
    int i = 0;
    
    while(true) {
        char c = next();
        if (c == '[') continue;
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == ',' || c == ']') {
            id[i] = 0x00;
            if (strcmp("r0", id) == 0) { return R0; }
            if (strcmp("r1", id) == 0) { return R1; }
            if (strcmp("r2", id) == 0) { return R2; }
            if (strcmp("r3", id) == 0) { return R3; }
            if (strcmp("sp", id) == 0) { return SP; }
            if (strcmp("pc", id) == 0) { return PC; }
            printf("Readed %s\n", id);
            break;
        }
        id[i] = c;
        i++;
    }
    return 99;
}

uint16_t match_hex_addr() {
    putBack(skip());
    char id[256] = {0};
    int i = 0;

    while(true) {
        char c = next();
        if (c == '[') continue;
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == ']') {
            id[i] = 0x00;
            return strtol(id, NULL, 16);
        }
        id[i] = c;
        i++;
    }
}

void analyze() {
    while(true) {
        int Token = match();
        switch(Token) {
            case T_eof: { return; }
            case T_entry: {
    entry_point = match_hex();
    //printf("Entry point stablished to %04x\n", entry_point);
    break;}
            case T_id: {
    //printf("Declaring a new id %s\n", current_id);
    int size = match_text();
    //printf("Declaring a new id %s -> %s\n", current_id, current_text);
    add_id(size);
    break; }
            case T_label: {
    add_label();
    break;
            }
            case T_var: {
    uint16_t num = match_hex();
    add_var(num);
    break;
            }
            case T_sys: {
    uint16_t num = match_hex();
    add_inst(I_sys, 0, 0, num, NULL, NULL, NULL, 2);
    break;
            }
            case T_sys_v: {
    match();
    add_inst(I_sys_v, 0, 0, 0, NULL, NULL, current_id, 2);
    break;
            }
            case T_break: {
    add_inst(I_break, 0, 0, 0, NULL, NULL, NULL, 1);
    break;
            }
            case T_call: {
    uint16_t num = match_hex();
    add_inst(I_call, 0, 0, num, NULL, NULL, NULL, 3);
    break; }
            case T_callr: {
    int reg = match_reg();
    add_inst(I_callr, reg, 0, 0, NULL, NULL, NULL, 1);
    break; }
            case T_call_v: {
    match();
    add_inst(I_call_v, 0, 0, 0, NULL, NULL, current_id, 3);
    break; }
            case T_jump: {
    uint16_t num = match_hex();
    add_inst(I_jump, 0, 0, num, NULL, NULL, NULL, 3);
    break; }
            case T_jump_t: {
    match();
    add_inst(I_jump_t, 0, 0, 0, NULL, current_id, NULL, 3);
    break; }
            case T_mov: {
    int reg = match_reg();
    int reg2 = match_reg();
    add_inst(I_mov, reg, reg2, 0, NULL, NULL, NULL, 1);
    break; }
            case T_movi: {
    int reg = match_reg();
    uint16_t num = match_hex();
    add_inst(I_movi, reg, 0, num, NULL, NULL, NULL, 3);
    break; }
            case T_movl: {
    int reg = match_reg();
    match();
    add_inst(I_movl, reg, 0, 0, current_id, NULL, NULL, 3);
    break; }
            case T_movt: {
    int reg = match_reg();
    match();
    add_inst(I_movt, reg, 0, 0, NULL, current_id, NULL, 3);
    break; }
            case T_movv: {
    int reg = match_reg();
    match();
    add_inst(I_movv, reg, 0, 0, NULL, NULL, current_id, 3);
    break; }
            case T_stb: {
    int reg = match_reg();
    int reg2 = match_reg_addr();
    add_inst(I_stb, reg, reg2, 0, NULL, NULL, NULL, 1);
    break; }
            case T_st: {
    int reg = match_reg();
    int reg2 = match_reg_addr();
    add_inst(I_st, reg, reg2, 0, NULL, NULL, NULL, 1);
    break; }
            case T_inc: {
    int reg = match_reg();
    add_inst(I_inc, reg, 0, 0, NULL, NULL, NULL, 1);
    break; }
            case T_dec: {
    int reg = match_reg();
    add_inst(I_dec, reg, 0, 0, NULL, NULL, NULL, 1);
    break; }
            case T_push: {
    int reg = match_reg();
    add_inst(I_push, reg, 0, 0, NULL, NULL, NULL, 1);
    break; }
            case T_pop: {
    int reg = match_reg();
    add_inst(I_pop, reg, 0, 0, NULL, NULL, NULL, 1);
    break; }
            case T_sti: {
    int reg = match_reg();
    uint16_t num = match_hex_addr();
    add_inst(I_sti, reg, 0, num, NULL, NULL, NULL, 3);
    break; }
            case T_stbi: {
    int reg = match_reg();
    uint16_t num = match_hex_addr();
    add_inst(I_stbi, reg, 0, num, NULL, NULL, NULL, 3);
    break; }
            case T_addi: {
    int reg = match_reg();
    uint16_t num = match_hex();
    add_inst(I_addi, reg, 0, num, NULL, NULL, NULL, 3);
    break; }
            case T_ld: {
    int reg = match_reg();
    int reg2 = match_reg_addr();
    add_inst(I_ld, reg, reg2, 0, NULL, NULL, NULL, 1);
    break; }
            case T_ldb: {
    int reg = match_reg();
    int reg2 = match_reg_addr();
    add_inst(I_ldb, reg, reg2, 0, NULL, NULL, NULL, 1);
    break; }
            case T_ldbi: {
    int reg = match_reg();
    uint16_t num = match_hex_addr();
    add_inst(I_ldbi, reg, 0, num, NULL, NULL, NULL, 3);
    break; }
            case T_ldi: {
    int reg = match_reg();
    uint16_t num = match_hex_addr();
    add_inst(I_ldi, reg, 0, num, NULL, NULL, NULL, 3);
    break; }
            case T_cmp: {
    int reg = match_reg();
    int reg2 = match_reg();
    add_inst(I_cmp, reg, reg2, 0, NULL, NULL, NULL, 1);
    break; }
            case T_cmpi: {
    int reg = match_reg();
    uint16_t num = match_hex();
    add_inst(I_cmpi, reg, 0, num, NULL, NULL, NULL, 3);
    break; }
            case T_jz: {
    uint16_t num = match_hex();
    add_inst(I_jz, 0, 0, num, NULL, NULL, NULL, 3);
    break; }
            case T_jz_t: {
    match();
    add_inst(I_jz_t, 0, 0, 0, NULL, current_id, NULL, 3);
    break; }
            case T_jnz: {
    uint16_t num = match_hex();
    add_inst(I_jnz, 0, 0, num, NULL, NULL, NULL, 3);
    break; }
            case T_jnz_t: {
    match();
    add_inst(I_jnz_t, 0, 0, 0, NULL, current_id, NULL, 3);
    break; }
            case T_jlt: {
    uint16_t num = match_hex();
    add_inst(I_jlt, 0, 0, num, NULL, NULL, NULL, 3);
    break; }
            case T_jgt: {
    uint16_t num = match_hex();
    add_inst(I_jgt, 0, 0, num, NULL, NULL, NULL, 3);
    break; }
            case T_call_z: {
    uint16_t num = match_hex();
    add_inst(I_call_z, 0, 0, num, NULL, NULL, NULL, 3);
    break; }
            case T_call_nz: {
    uint16_t num = match_hex();
    add_inst(I_call_nz, 0, 0, num, NULL, NULL, NULL, 3);
    break; }
            case T_call_g: {
    uint16_t num = match_hex();
    add_inst(I_call_g, 0, 0, num, NULL, NULL, NULL, 3);
    break; }
            case T_call_l: {
    uint16_t num = match_hex();
    add_inst(I_call_l, 0, 0, num, NULL, NULL, NULL, 3);
    break; }
            case T_ret: {
    add_inst(I_ret, 0, 0, 0, NULL, NULL, NULL, 1);
    break; }
        }
    }
}

void dump_call(int i) {
    fwrite("\x99", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output);
}

void dump_callr(int i) {
    switch(inst[i].reg) {
        case R0: fwrite("\xc9", 1, 1, output);
        case R1: fwrite("\xca", 1, 1, output);
        case R2: fwrite("\xcb", 1, 1, output);
        case R3: fwrite("\xcc", 1, 1, output);
    }
}

void dump_call_v(int i) {
    fwrite("\x99", 1, 1, output); fwrite(&(find_var(inst[i].var)->addr), 2, 1, output);
}

void dump_jump(int i) {
    fwrite("\x98", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output);
}

void dump_jump_t(int i) {
    fwrite("\x98", 1, 1, output); fwrite(&(find_label(inst[i].label)->addr), 2, 1, output);
}

void dump_break(int i) {
    fwrite("\x00", 1, 1, output);
}

void dump_ret(int i) {
    fwrite("\x05", 1, 1, output);
}

void dump_sys(int i) {
    fwrite("\x06", 1, 1, output); fwrite(&(inst[i].imm), 1, 1, output);
}

void dump_sys_v(int i) {
    fwrite("\x06", 1, 1, output); fwrite(&(find_var(inst[i].var)->addr), 1, 1, output);
}

void dump_jz(int i) {
    fwrite("\x9c", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output);
}

void dump_jz_t(int i) {
    fwrite("\x9c", 1, 1, output); fwrite(&(find_label(inst[i].label)->addr), 2, 1, output);
}

void dump_jnz(int i) {
    fwrite("\x9d", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output);
}

void dump_jnz_t(int i) {
    fwrite("\x9d", 1, 1, output); fwrite(&(find_label(inst[i].label)->addr), 2, 1, output);
}

void dump_jgt(int i) {
    fwrite("\x9b", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output);
}

void dump_jlt(int i) {
    fwrite("\x9a", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output);
}

void dump_call_z(int i) {
    fwrite("\xa0", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output);
}

void dump_call_nz(int i) {
    fwrite("\xa1", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output);
}

void dump_call_g(int i) {
    fwrite("\x9f", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output);
}

void dump_call_l(int i) {
    fwrite("\x9e", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output);
}

void dump_mov(int i) {
    switch(inst[i].reg) {
        case R0:
            switch(inst[i].reg2) {
            case SP: fwrite("\x0a", 1, 1, output); break;
            case R0: fwrite("\x20", 1, 1, output); break;
            case R1: fwrite("\x21", 1, 1, output); break;
            case R2: fwrite("\x22", 1, 1, output); break;
            case R3: fwrite("\x23", 1, 1, output); break;
            }
            break;
        case R1:
            switch(inst[i].reg2) {
            case R0: fwrite("\x24", 1, 1, output); break;
            case R1: fwrite("\x25", 1, 1, output); break;
            case R2: fwrite("\x26", 1, 1, output); break;
            case R3: fwrite("\x27", 1, 1, output); break;
            }
            break;
        case R2:
            switch(inst[i].reg2) {
            case R0: fwrite("\x28", 1, 1, output); break;
            case R1: fwrite("\x29", 1, 1, output); break;
            case R2: fwrite("\x2a", 1, 1, output); break;
            case R3: fwrite("\x2b", 1, 1, output); break;
            }
            break;
        case R3:
            switch(inst[i].reg2) {
            case R0: fwrite("\x2c", 1, 1, output); break;
            case R1: fwrite("\x2d", 1, 1, output); break;
            case R2: fwrite("\x2e", 1, 1, output); break;
            case R3: fwrite("\x2f", 1, 1, output); break;
            }
            break;
    }
}

void dump_movi(int i) {
    switch(inst[i].reg) {
        case SP: fwrite("\x09", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
        case R0: fwrite("\x10", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
        case R1: fwrite("\x11", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
        case R2: fwrite("\x12", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
        case R3: fwrite("\x13", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
    }
}

void dump_addi(int i) {
    switch(inst[i].reg) {
        case R0: fwrite("\xe0", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
        case R1: fwrite("\xe1", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
        case R2: fwrite("\xe2", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
        case R3: fwrite("\xe3", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
    }
}

void dump_ldb(int i) {
    switch(inst[i].reg) {
        case R0:
            switch(inst[i].reg2) {
            case R0: fwrite("\x40", 1, 1, output); break;
            case R1: fwrite("\x41", 1, 1, output); break;
            case R2: fwrite("\x42", 1, 1, output); break;
            case R3: fwrite("\x43", 1, 1, output); break;
            }
            break;
        case R1:
            switch(inst[i].reg2) {
            case R0: fwrite("\x44", 1, 1, output); break;
            case R1: fwrite("\x45", 1, 1, output); break;
            case R2: fwrite("\x46", 1, 1, output); break;
            case R3: fwrite("\x47", 1, 1, output); break;
            }
            break;
        case R2:
            switch(inst[i].reg2) {
            case R0: fwrite("\x48", 1, 1, output); break;
            case R1: fwrite("\x49", 1, 1, output); break;
            case R2: fwrite("\x4a", 1, 1, output); break;
            case R3: fwrite("\x4b", 1, 1, output); break;
            }
            break;
        case R3:
            switch(inst[i].reg2) {
            case R0: fwrite("\x4c", 1, 1, output); break;
            case R1: fwrite("\x4d", 1, 1, output); break;
            case R2: fwrite("\x4e", 1, 1, output); break;
            case R3: fwrite("\x4f", 1, 1, output); break;
            }
            break;
    }
}

void dump_ld(int i) {
    switch(inst[i].reg) {
        case R0:
            switch(inst[i].reg2) {
            case R0: fwrite("\x50", 1, 1, output); break;
            case R1: fwrite("\x51", 1, 1, output); break;
            case R2: fwrite("\x52", 1, 1, output); break;
            case R3: fwrite("\x53", 1, 1, output); break;
            }
            break;
        case R1:
            switch(inst[i].reg2) {
            case R0: fwrite("\x54", 1, 1, output); break;
            case R1: fwrite("\x55", 1, 1, output); break;
            case R2: fwrite("\x56", 1, 1, output); break;
            case R3: fwrite("\x57", 1, 1, output); break;
            }
            break;
        case R2:
            switch(inst[i].reg2) {
            case R0: fwrite("\x58", 1, 1, output); break;
            case R1: fwrite("\x59", 1, 1, output); break;
            case R2: fwrite("\x5a", 1, 1, output); break;
            case R3: fwrite("\x5b", 1, 1, output); break;
            }
            break;
        case R3:
            switch(inst[i].reg2) {
            case R0: fwrite("\x5c", 1, 1, output); break;
            case R1: fwrite("\x5d", 1, 1, output); break;
            case R2: fwrite("\x5e", 1, 1, output); break;
            case R3: fwrite("\x5f", 1, 1, output); break;
            }
            break;
    }
}

void dump_ldi(int i) {
    switch(inst[i].reg) {
        case R0: fwrite("\xb4", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
        case R1: fwrite("\xb5", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
        case R2: fwrite("\xb6", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
        case R3: fwrite("\xb7", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
    }
}

void dump_ldbi(int i) {
    switch(inst[i].reg) {
        case R0: fwrite("\xb0", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
        case R1: fwrite("\xb1", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
        case R2: fwrite("\xb2", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
        case R3: fwrite("\xb3", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
    }
}

void dump_push(int i) {
    switch(inst[i].reg) {
    case SP: fwrite("\xa6", 1, 1, output); break;
    case PC: fwrite("\xa7", 1, 1, output); break;
    case R0: fwrite("\x90", 1, 1, output); break;
    case R1: fwrite("\x91", 1, 1, output); break;
    case R2: fwrite("\x92", 1, 1, output); break;
    case R3: fwrite("\x93", 1, 1, output); break;
    }
}

void dump_pop(int i) {
    switch(inst[i].reg) {
    case R0: fwrite("\x94", 1, 1, output); break;
    case R1: fwrite("\x95", 1, 1, output); break;
    case R2: fwrite("\x96", 1, 1, output); break;
    case R3: fwrite("\x97", 1, 1, output); break;
    }
}

void dump_movl(int i) {
    switch(inst[i].reg) {
    case SP: fwrite("\x09", 1, 1, output); fwrite(&(find_id(inst[i].id)->addr), 2, 1, output); break;
    case R0: fwrite("\x10", 1, 1, output); fwrite(&(find_id(inst[i].id)->addr), 2, 1, output); break;
    case R1: fwrite("\x11", 1, 1, output); fwrite(&(find_id(inst[i].id)->addr), 2, 1, output); break;
    case R2: fwrite("\x12", 1, 1, output); fwrite(&(find_id(inst[i].id)->addr), 2, 1, output); break;
    case R3: fwrite("\x13", 1, 1, output); fwrite(&(find_id(inst[i].id)->addr), 2, 1, output); break;
    }
}

void dump_movv(int i) {
    switch(inst[i].reg) {
    case SP: fwrite("\x09", 1, 1, output); fwrite(&(find_var(inst[i].var)->addr), 2, 1, output); break;
    case R0: fwrite("\x10", 1, 1, output); fwrite(&(find_var(inst[i].var)->addr), 2, 1, output); break;
    case R1: fwrite("\x11", 1, 1, output); fwrite(&(find_var(inst[i].var)->addr), 2, 1, output); break;
    case R2: fwrite("\x12", 1, 1, output); fwrite(&(find_var(inst[i].var)->addr), 2, 1, output); break;
    case R3: fwrite("\x13", 1, 1, output); fwrite(&(find_var(inst[i].var)->addr), 2, 1, output); break;
    }
}

void dump_movt(int i) {
    switch(inst[i].reg) {
    case SP: fwrite("\x09", 1, 1, output); fwrite(&(find_label(inst[i].label)->addr), 2, 1, output); break;
    case R0: fwrite("\x10", 1, 1, output); fwrite(&(find_label(inst[i].label)->addr), 2, 1, output); break;
    case R1: fwrite("\x11", 1, 1, output); fwrite(&(find_label(inst[i].label)->addr), 2, 1, output); break;
    case R2: fwrite("\x12", 1, 1, output); fwrite(&(find_label(inst[i].label)->addr), 2, 1, output); break;
    case R3: fwrite("\x13", 1, 1, output); fwrite(&(find_label(inst[i].label)->addr), 2, 1, output); break;
    }
}

void dump_inc(int i) {
    switch(inst[i].reg) {
        case R0: fwrite("\xa8", 1, 1, output); break;
        case R1: fwrite("\xa9", 1, 1, output); break;
        case R2: fwrite("\xaa", 1, 1, output); break;
        case R3: fwrite("\xab", 1, 1, output); break;
    }
}

void dump_dec(int i) {
    switch(inst[i].reg) {
        case R0: fwrite("\xac", 1, 1, output); break;
        case R1: fwrite("\xad", 1, 1, output); break;
        case R2: fwrite("\xae", 1, 1, output); break;
        case R3: fwrite("\xaf", 1, 1, output); break;
    }
}

void dump_stb(int i) {
    switch(inst[i].reg2) {
        case R0:
            switch(inst[i].reg) {
            case R0: fwrite("\x60", 1, 1, output); break;
            case R1: fwrite("\x61", 1, 1, output); break;
            case R2: fwrite("\x62", 1, 1, output); break;
            case R3: fwrite("\x63", 1, 1, output); break;
            }
            break;
        case R1:
            switch(inst[i].reg) {
            case R0: fwrite("\x64", 1, 1, output); break;
            case R1: fwrite("\x65", 1, 1, output); break;
            case R2: fwrite("\x66", 1, 1, output); break;
            case R3: fwrite("\x67", 1, 1, output); break;
            }
            break;
        case R2:
            switch(inst[i].reg) {
            case R0: fwrite("\x68", 1, 1, output); break;
            case R1: fwrite("\x69", 1, 1, output); break;
            case R2: fwrite("\x6a", 1, 1, output); break;
            case R3: fwrite("\x6b", 1, 1, output); break;
            }
            break;
        case R3:
            switch(inst[i].reg) {
            case R0: fwrite("\x6c", 1, 1, output); break;
            case R1: fwrite("\x6d", 1, 1, output); break;
            case R2: fwrite("\x6e", 1, 1, output); break;
            case R3: fwrite("\x6f", 1, 1, output); break;
            }
            break;
    }
}

void dump_st(int i) {
    switch(inst[i].reg2) {
        case R0:
            switch(inst[i].reg) {
            case R0: fwrite("\x70", 1, 1, output); break;
            case R1: fwrite("\x71", 1, 1, output); break;
            case R2: fwrite("\x72", 1, 1, output); break;
            case R3: fwrite("\x73", 1, 1, output); break;
            }
            break;
        case R1:
            switch(inst[i].reg) {
            case R0: fwrite("\x74", 1, 1, output); break;
            case R1: fwrite("\x75", 1, 1, output); break;
            case R2: fwrite("\x76", 1, 1, output); break;
            case R3: fwrite("\x77", 1, 1, output); break;
            }
            break;
        case R2:
            switch(inst[i].reg) {
            case R0: fwrite("\x78", 1, 1, output); break;
            case R1: fwrite("\x79", 1, 1, output); break;
            case R2: fwrite("\x7a", 1, 1, output); break;
            case R3: fwrite("\x7b", 1, 1, output); break;
            }
            break;
        case R3:
            switch(inst[i].reg) {
            case R0: fwrite("\x7c", 1, 1, output); break;
            case R1: fwrite("\x7d", 1, 1, output); break;
            case R2: fwrite("\x7e", 1, 1, output); break;
            case R3: fwrite("\x7f", 1, 1, output); break;
            }
            break;
    }
}

void dump_cmp(int i) {
    switch(inst[i].reg) {
        case R0:
            switch(inst[i].reg2) {
            case R0: fwrite("\x80", 1, 1, output); break;
            case R1: fwrite("\x81", 1, 1, output); break;
            case R2: fwrite("\x82", 1, 1, output); break;
            case R3: fwrite("\x83", 1, 1, output); break;
            }
            break;
        case R1:
            switch(inst[i].reg2) {
            case R0: fwrite("\x84", 1, 1, output); break;
            case R1: fwrite("\x85", 1, 1, output); break;
            case R2: fwrite("\x86", 1, 1, output); break;
            case R3: fwrite("\x87", 1, 1, output); break;
            }
            break;
        case R2:
            switch(inst[i].reg2) {
            case R0: fwrite("\x88", 1, 1, output); break;
            case R1: fwrite("\x89", 1, 1, output); break;
            case R2: fwrite("\x8a", 1, 1, output); break;
            case R3: fwrite("\x8b", 1, 1, output); break;
            }
            break;
        case R3:
            switch(inst[i].reg2) {
            case R0: fwrite("\x8c", 1, 1, output); break;
            case R1: fwrite("\x8d", 1, 1, output); break;
            case R2: fwrite("\x8e", 1, 1, output); break;
            case R3: fwrite("\x8f", 1, 1, output); break;
            }
            break;
    }
}

void dump_cmpi(int i) {
    switch(inst[i].reg) {
        case R0: fwrite("\xa2", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
        case R1: fwrite("\xa3", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
        case R2: fwrite("\xa4", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
        case R3: fwrite("\xa5", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
    }
}

void dump_sti(int i) {
    switch(inst[i].reg) {
        case R0: fwrite("\xbc", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
        case R1: fwrite("\xbd", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
        case R2: fwrite("\xbe", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
        case R3: fwrite("\xbf", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
    }
}

void dump_stbi(int i) {
    switch(inst[i].reg) {
        case R0: fwrite("\xb8", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
        case R1: fwrite("\xb9", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
        case R2: fwrite("\xba", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
        case R3: fwrite("\xbb", 1, 1, output); fwrite(&(inst[i].imm), 2, 1, output); break;
    }
}

void dump_code() {
    for (int i = 0; i <= curr_inst; ++i) {
        printf("Dumping instruction %d/%d\n", i, curr_inst);
        switch(inst[i].type) {
        case I_break: dump_break(i); break;
        case I_sys: dump_sys(i); break;
        case I_sys_v: dump_sys_v(i); break;
        case I_mov: dump_mov(i); break;
        case I_movi: dump_movi(i); break;
        case I_movl: dump_movl(i); break;
        case I_movt: dump_movt(i); break;
        case I_movv: dump_movv(i); break;
        case I_st: dump_st(i); break;
        case I_sti: dump_sti(i); break;
        case I_stb: dump_stb(i); break;
        case I_stbi: dump_stbi(i); break;
        case I_inc: dump_inc(i); break;
        case I_dec: dump_dec(i); break;
        case I_call: dump_call(i); break;
        case I_callr: dump_callr(i); break;
        case I_call_v: dump_call_v(i); break;
        case I_jump: dump_jump(i); break;
        case I_jump_t: dump_jump_t(i); break;
        case I_push: dump_push(i); break;
        case I_ld: dump_ld(i); break;
        case I_ldi: dump_ldi(i); break;
        case I_ldb: dump_ldb(i); break;
        case I_ldbi: dump_ldbi(i); break;
        case I_pop: dump_pop(i); break;
        case I_addi: dump_addi(i); break;
        case I_cmp: dump_cmp(i); break;
        case I_cmpi: dump_cmpi(i); break;
        case I_jz: dump_jz(i); break;
        case I_jz_t: dump_jz_t(i); break;
        case I_jnz: dump_jnz(i); break;
        case I_jnz_t: dump_jnz_t(i); break;
        case I_jgt: dump_jgt(i); break;
        case I_jlt: dump_jlt(i); break;
        case I_call_z: dump_call_z(i); break;
        case I_call_nz: dump_call_nz(i); break;
        case I_call_g: dump_call_g(i); break;
        case I_call_l: dump_call_l(i); break;
        case I_ret: dump_ret(i); break;
        case I_end: return;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) return 1;
    
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

    analyze();
    calculate_end_of_code();
    printf("Code ends at %04x\n", eoc);
    reserve_id_space();
    dump_code();
    dump_identifiers();
    
    fclose(input);
    fclose(output);
    return 0;
}
