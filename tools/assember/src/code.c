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

#include "file.h"
#include "lex.h"
#include "gram.h"
#include "code.h"
#include "symbols.h"

Instruction instructions = {
    .instruction = T_Unknown,
    .next = NULL,
};
uint16_t entry_point;
uint16_t end_of_code;

void setEntryPoint(uint16_t ep) {
    entry_point = ep;
}

void calculate_opcode_and_size_jump(Instruction* node) {
    switch(node->reg) {
        case R0: node->opcode = 0xc5; node->inst_size = 1; return;
        case R1: node->opcode = 0xc6; node->inst_size = 1; return;
        case R2: node->opcode = 0xc7; node->inst_size = 1; return;
        case R3: node->opcode = 0xc8; node->inst_size = 1; return;
        case R_Unknown:
            if (node->label || node->text || node->define || node->imm || node->expression.type == T_expression) {
                node->opcode = 0x98; node->inst_size = 3; return;
            }
            else { assert(false); }
        default: assert(false);
    }
}
void calculate_opcode_and_size_call(Instruction* node) {
    switch(node->reg) {
        case R0: node->opcode = 0xc9; node->inst_size = 1; return;
        case R1: node->opcode = 0xca; node->inst_size = 1; return;
        case R2: node->opcode = 0xcb; node->inst_size = 1; return;
        case R3: node->opcode = 0xcc; node->inst_size = 1; return;
        case R_Unknown:
            if (node->label || node->text || node->define || node->imm || node->expression.type == T_expression) {
                node->opcode = 0x99; node->inst_size = 3; return;
            }
            else { assert(false); }
        default: assert(false);
    }
}
void calculate_opcode_and_size_cmp(Instruction* node) {
    switch(node->regm) {
        case R0:
            switch(node->reg) {
                case R0: node->opcode = 0x80; node->inst_size = 1; return;
                case R1: node->opcode = 0x84; node->inst_size = 1; return;
                case R2: node->opcode = 0x88; node->inst_size = 1; return;
                case R3: node->opcode = 0x8c; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R1:
            switch(node->reg) {
                case R0: node->opcode = 0x81; node->inst_size = 1; return;
                case R1: node->opcode = 0x85; node->inst_size = 1; return;
                case R2: node->opcode = 0x89; node->inst_size = 1; return;
                case R3: node->opcode = 0x8d; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R2:
            switch(node->reg) {
                case R0: node->opcode = 0x82; node->inst_size = 1; return;
                case R1: node->opcode = 0x86; node->inst_size = 1; return;
                case R2: node->opcode = 0x8a; node->inst_size = 1; return;
                case R3: node->opcode = 0x8e; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R3:
            switch(node->reg) {
                case R0: node->opcode = 0x83; node->inst_size = 1; return;
                case R1: node->opcode = 0x87; node->inst_size = 1; return;
                case R2: node->opcode = 0x8b; node->inst_size = 1; return;
                case R3: node->opcode = 0x8f; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R_Unknown: 
            if (node->label || node->text || node->define || node->imm || node->expression.type == T_expression) {
                switch(node->reg) {
                    case R0: node->opcode = 0xa2; node->inst_size = 3; return;
                    case R1: node->opcode = 0xa3; node->inst_size = 3; return;
                    case R2: node->opcode = 0xa4; node->inst_size = 3; return;
                    case R3: node->opcode = 0xa5; node->inst_size = 3; return;
                    default: assert(false);
                }
            }
            else { assert(false); }
            return;
        default: assert(false);
    }
}
void calculate_opcode_and_size_and(Instruction* node) {
    switch(node->reg) {
        case R0:
            switch(node->regm) {
                case R0: node->opcode = 0xd0; node->inst_size = 1; return;
                case R1: node->opcode = 0xd1; node->inst_size = 1; return;
                case R2: node->opcode = 0xd2; node->inst_size = 1; return;
                case R3: node->opcode = 0xd3; node->inst_size = 1; return;
                case R_Unknown:
                    if (node->label || node->text || node->define || node->imm || node->expression.type == T_expression) {
                        node->opcode = 0xcd; node->inst_size = 3; return;
                    }
                    else { assert(false); }
            }
        default: assert(false);
    }
}
void calculate_opcode_and_size_or(Instruction* node) {
    switch(node->reg) {
        case R0:
            switch(node->regm) {
                case R0: node->opcode = 0xd4; node->inst_size = 1; return;
                case R1: node->opcode = 0xd5; node->inst_size = 1; return;
                case R2: node->opcode = 0xd6; node->inst_size = 1; return;
                case R3: node->opcode = 0xd7; node->inst_size = 1; return;
                case R_Unknown:
                    if (node->label || node->text || node->define || node->imm || node->expression.type == T_expression) {
                        node->opcode = 0xce; node->inst_size = 3; return;
                    }
                    else { assert(false); }
            }
        default: assert(false);
    }
}
void calculate_opcode_and_size_xor(Instruction* node) {
    switch(node->reg) {
        case R0:
            switch(node->regm) {
                case R0: node->opcode = 0xd8; node->inst_size = 1; return;
                case R1: node->opcode = 0xd9; node->inst_size = 1; return;
                case R2: node->opcode = 0xda; node->inst_size = 1; return;
                case R3: node->opcode = 0xdb; node->inst_size = 1; return;
                case R_Unknown:
                    if (node->label || node->text || node->define || node->imm || node->expression.type == T_expression) {
                        node->opcode = 0xcf; node->inst_size = 3; return;
                    }
                    else { assert(false); }
            }
        default: assert(false);
    }
}
void calculate_opcode_and_size_push(Instruction* node) {
    switch(node->reg) {
        case PC: node->opcode = 0xa7; node->inst_size = 1; return;
        case SP: node->opcode = 0xa6; node->inst_size = 1; return;
        case R0: node->opcode = 0x90; node->inst_size = 1; return;
        case R1: node->opcode = 0x91; node->inst_size = 1; return;
        case R2: node->opcode = 0x92; node->inst_size = 1; return;
        case R3: node->opcode = 0x93; node->inst_size = 1; return;
        default: assert(false);
    } return;
}
void calculate_opcode_and_size_pop(Instruction* node) {
    switch(node->reg) {
        case R0: node->opcode = 0x94; node->inst_size = 1; return;
        case R1: node->opcode = 0x95; node->inst_size = 1; return;
        case R2: node->opcode = 0x96; node->inst_size = 1; return;
        case R3: node->opcode = 0x97; node->inst_size = 1; return;
        default: assert(false);
    } return;
}
void calculate_opcode_and_size_mul(Instruction* node) {
    switch(node->reg) {
        case R0: 
            switch (node->regm) {
                case R_Unknown:
                    if (node->label || node->text || node->define || node->imm || node->expression.type == T_expression) {
                        node->opcode = 0x01; node->inst_size = 3;
                    }
                    else { assert(false); }
                    return;
                case R1: node->opcode = 0x02; node->inst_size = 1; return;
                default: assert(false);
            }
        default: assert(false);
    } return;
}
void calculate_opcode_and_size_div(Instruction* node) {
    switch(node->reg) {
        case R0: 
            switch (node->regm) {
                case R_Unknown:
                    if (node->label || node->text || node->define || node->imm || node->expression.type == T_expression) {
                        node->opcode = 0x03; node->inst_size = 3;
                    }
                    else { assert(false); }
                    return;
                case R1: node->opcode = 0x04; node->inst_size = 1; return;
                default: assert(false);
            }
        default: assert(false);
    } return;
}
void calculate_opcode_and_size_inc(Instruction* node) {
    switch(node->reg) {
        case R0: node->opcode = 0xa8; node->inst_size = 1; return;
        case R1: node->opcode = 0xa9; node->inst_size = 1; return;
        case R2: node->opcode = 0xaa; node->inst_size = 1; return;
        case R3: node->opcode = 0xab; node->inst_size = 1; return;
        default: assert(false);
    } 
}
void calculate_opcode_and_size_dec(Instruction* node) {
    switch(node->reg) {
        case R0: node->opcode = 0xac; node->inst_size = 1; return;
        case R1: node->opcode = 0xad; node->inst_size = 1; return;
        case R2: node->opcode = 0xae; node->inst_size = 1; return;
        case R3: node->opcode = 0xaf; node->inst_size = 1; return;
        default: assert(false);
    } 
}
void calculate_opcode_and_size_add(Instruction* node) {
    switch(node->regm) {
        case R0:
            switch(node->reg) {
                case R0: node->opcode = 0x30; node->inst_size = 1; return;
                case R1: node->opcode = 0x34; node->inst_size = 1; return;
                case R2: node->opcode = 0x38; node->inst_size = 1; return;
                case R3: node->opcode = 0x3c; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R1:
            switch(node->reg) {
                case R0: node->opcode = 0x31; node->inst_size = 1; return;
                case R1: node->opcode = 0x35; node->inst_size = 1; return;
                case R2: node->opcode = 0x39; node->inst_size = 1; return;
                case R3: node->opcode = 0x3d; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R2:
            switch(node->reg) {
                case R0: node->opcode = 0x32; node->inst_size = 1; return;
                case R1: node->opcode = 0x36; node->inst_size = 1; return;
                case R2: node->opcode = 0x3a; node->inst_size = 1; return;
                case R3: node->opcode = 0x3e; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R3:
            switch(node->reg) {
                case R0: node->opcode = 0x33; node->inst_size = 1; return;
                case R1: node->opcode = 0x37; node->inst_size = 1; return;
                case R2: node->opcode = 0x3b; node->inst_size = 1; return;
                case R3: node->opcode = 0x3f; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R_Unknown: 
            if (node->label || node->text || node->define || node->imm || node->expression.type == T_expression) {
                switch(node->reg) {
                    case R0: node->opcode = 0xe0; node->inst_size = 3; return;
                    case R1: node->opcode = 0xe1; node->inst_size = 3; return;
                    case R2: node->opcode = 0xe2; node->inst_size = 3; return;
                    case R3: node->opcode = 0xe3; node->inst_size = 3; return;
                    default: assert(false);
                }
            }
            else { assert(false); }
            return;
        default: assert(false);
    }
}
void calculate_opcode_and_size_mov(Instruction* node) {
    switch(node->regm) {
        case R0:
            switch(node->reg) {
                case R0: node->opcode = 0x20; node->inst_size = 1; return;
                case R1: node->opcode = 0x24; node->inst_size = 1; return;
                case R2: node->opcode = 0x28; node->inst_size = 1; return;
                case R3: node->opcode = 0x2c; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R1:
            switch(node->reg) {
                case R0: node->opcode = 0x21; node->inst_size = 1; return;
                case R1: node->opcode = 0x25; node->inst_size = 1; return;
                case R2: node->opcode = 0x29; node->inst_size = 1; return;
                case R3: node->opcode = 0x2d; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R2:
            switch(node->reg) {
                case R0: node->opcode = 0x22; node->inst_size = 1; return;
                case R1: node->opcode = 0x26; node->inst_size = 1; return;
                case R2: node->opcode = 0x2a; node->inst_size = 1; return;
                case R3: node->opcode = 0x2e; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R3:
            switch(node->reg) {
                case R0: node->opcode = 0x23; node->inst_size = 1; return;
                case R1: node->opcode = 0x27; node->inst_size = 1; return;
                case R2: node->opcode = 0x2b; node->inst_size = 1; return;
                case R3: node->opcode = 0x2f; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case SP:
            switch(node->reg) {
                case R0: node->opcode = 0x0a; node->inst_size = 1; return;
                case R1: node->opcode = 0x0b; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R_Unknown: 
            if (node->label || node->text || node->define || node->imm || node->expression.type == T_expression) {
                switch(node->reg) {
                    case SP: node->opcode = 0x09; node->inst_size = 3; return;
                    case R0: node->opcode = 0x10; node->inst_size = 3; return;
                    case R1: node->opcode = 0x11; node->inst_size = 3; return;
                    case R2: node->opcode = 0x12; node->inst_size = 3; return;
                    case R3: node->opcode = 0x13; node->inst_size = 3; return;
                    default: assert(false);
                }
            }
            else { assert(false); }
            return;
        default: assert(false);
    }
}
void calculate_opcode_and_size_ld(Instruction* node) {
    switch(node->regm) {
        case R0:
            switch(node->reg) {
                case R0: node->opcode = 0x50; node->inst_size = 1; return;
                case R1: node->opcode = 0x54; node->inst_size = 1; return;
                case R2: node->opcode = 0x58; node->inst_size = 1; return;
                case R3: node->opcode = 0x5c; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R1:
            switch(node->reg) {
                case R0: node->opcode = 0x51; node->inst_size = 1; return;
                case R1: node->opcode = 0x55; node->inst_size = 1; return;
                case R2: node->opcode = 0x59; node->inst_size = 1; return;
                case R3: node->opcode = 0x5d; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R2:
            switch(node->reg) {
                case R0: node->opcode = 0x52; node->inst_size = 1; return;
                case R1: node->opcode = 0x56; node->inst_size = 1; return;
                case R2: node->opcode = 0x5a; node->inst_size = 1; return;
                case R3: node->opcode = 0x5e; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R3:
            switch(node->reg) {
                case R0: node->opcode = 0x53; node->inst_size = 1; return;
                case R1: node->opcode = 0x57; node->inst_size = 1; return;
                case R2: node->opcode = 0x5b; node->inst_size = 1; return;
                case R3: node->opcode = 0x5f; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R_Unknown: 
            if (node->label || node->text || node->define || node->imm || node->expression.type == T_expression) {
                switch(node->reg) {
                    case R0: node->opcode = 0xb4; node->inst_size = 3; return;
                    case R1: node->opcode = 0xb5; node->inst_size = 3; return;
                    case R2: node->opcode = 0xb6; node->inst_size = 3; return;
                    case R3: node->opcode = 0xb7; node->inst_size = 3; return;
                    default: assert(false);
                }
            }
            else { assert(false); }
            return;
        default: assert(false);
    }
}
void calculate_opcode_and_size_ldb(Instruction* node) {
    switch(node->regm) {
        case R0:
            switch(node->reg) {
                case R0: node->opcode = 0x40; node->inst_size = 1; return;
                case R1: node->opcode = 0x44; node->inst_size = 1; return;
                case R2: node->opcode = 0x48; node->inst_size = 1; return;
                case R3: node->opcode = 0x4c; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R1:
            switch(node->reg) {
                case R0: node->opcode = 0x41; node->inst_size = 1; return;
                case R1: node->opcode = 0x45; node->inst_size = 1; return;
                case R2: node->opcode = 0x49; node->inst_size = 1; return;
                case R3: node->opcode = 0x4d; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R2:
            switch(node->reg) {
                case R0: node->opcode = 0x42; node->inst_size = 1; return;
                case R1: node->opcode = 0x46; node->inst_size = 1; return;
                case R2: node->opcode = 0x4a; node->inst_size = 1; return;
                case R3: node->opcode = 0x4c; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R3:
            switch(node->reg) {
                case R0: node->opcode = 0x43; node->inst_size = 1; return;
                case R1: node->opcode = 0x47; node->inst_size = 1; return;
                case R2: node->opcode = 0x4b; node->inst_size = 1; return;
                case R3: node->opcode = 0x4f; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R_Unknown: 
            if (node->label || node->text || node->define || node->imm || node->expression.type == T_expression) {
                switch(node->reg) {
                    case R0: node->opcode = 0xb0; node->inst_size = 3; return;
                    case R1: node->opcode = 0xb1; node->inst_size = 3; return;
                    case R2: node->opcode = 0xb2; node->inst_size = 3; return;
                    case R3: node->opcode = 0xb3; node->inst_size = 3; return;
                    default: assert(false);
                }
            }
            else { assert(false); }
            return;
        default: assert(false);
    }
}
void calculate_opcode_and_size_st(Instruction* node) {
    switch(node->regm) {
        case R0:
            switch(node->reg) {
                case R0: node->opcode = 0x70; node->inst_size = 1; return;
                case R1: node->opcode = 0x71; node->inst_size = 1; return;
                case R2: node->opcode = 0x72; node->inst_size = 1; return;
                case R3: node->opcode = 0x73; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R1:
            switch(node->reg) {
                case R0: node->opcode = 0x74; node->inst_size = 1; return;
                case R1: node->opcode = 0x75; node->inst_size = 1; return;
                case R2: node->opcode = 0x76; node->inst_size = 1; return;
                case R3: node->opcode = 0x77; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R2:
            switch(node->reg) {
                case R0: node->opcode = 0x78; node->inst_size = 1; return;
                case R1: node->opcode = 0x79; node->inst_size = 1; return;
                case R2: node->opcode = 0x7a; node->inst_size = 1; return;
                case R3: node->opcode = 0x7b; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R3:
            switch(node->reg) {
                case R0: node->opcode = 0x7c; node->inst_size = 1; return;
                case R1: node->opcode = 0x7d; node->inst_size = 1; return;
                case R2: node->opcode = 0x7e; node->inst_size = 1; return;
                case R3: node->opcode = 0x7f; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R_Unknown: 
            if (node->label || node->text || node->define || node->imm || node->expression.type == T_expression) {
                switch(node->reg) {
                    case R0: node->opcode = 0xbc; node->inst_size = 3; return;
                    case R1: node->opcode = 0xbd; node->inst_size = 3; return;
                    case R2: node->opcode = 0xbe; node->inst_size = 3; return;
                    case R3: node->opcode = 0xbf; node->inst_size = 3; return;
                    default: assert(false);
                }
            }
            else { assert(false); }
            return;
        default: assert(false);
    }
}
void calculate_opcode_and_size_stb(Instruction* node) {
    switch(node->regm) {
        case R0:
            switch(node->reg) {
                case R0: node->opcode = 0x60; node->inst_size = 1; return;
                case R1: node->opcode = 0x61; node->inst_size = 1; return;
                case R2: node->opcode = 0x62; node->inst_size = 1; return;
                case R3: node->opcode = 0x63; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R1:
            switch(node->reg) {
                case R0: node->opcode = 0x64; node->inst_size = 1; return;
                case R1: node->opcode = 0x65; node->inst_size = 1; return;
                case R2: node->opcode = 0x66; node->inst_size = 1; return;
                case R3: node->opcode = 0x67; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R2:
            switch(node->reg) {
                case R0: node->opcode = 0x68; node->inst_size = 1; return;
                case R1: node->opcode = 0x69; node->inst_size = 1; return;
                case R2: node->opcode = 0x6a; node->inst_size = 1; return;
                case R3: node->opcode = 0x6b; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R3:
            switch(node->reg) {
                case R0: node->opcode = 0x6c; node->inst_size = 1; return;
                case R1: node->opcode = 0x6d; node->inst_size = 1; return;
                case R2: node->opcode = 0x6e; node->inst_size = 1; return;
                case R3: node->opcode = 0x6f; node->inst_size = 1; return;
                default: assert(false);
            } 
            return;
        case R_Unknown: 
            if (node->label || node->text || node->define || node->imm || node->expression.type == T_expression) {
                switch(node->reg) {
                    case R0: node->opcode = 0xb8; node->inst_size = 3; return;
                    case R1: node->opcode = 0xb9; node->inst_size = 3; return;
                    case R2: node->opcode = 0xba; node->inst_size = 3; return;
                    case R3: node->opcode = 0xbb; node->inst_size = 3; return;
                    default: assert(false);
                }
            }
            else { assert(false); }
            return;
        default: assert(false);
    }
}

void calculate_opcode_and_size(Instruction* node) {
    switch(node->instruction) {
        case T_I_break: node->opcode = 0x00; node->inst_size = 1; return;
        case T_I_ret:   node->opcode = 0x05; node->inst_size = 1; return;
        case T_I_nop:   node->opcode = 0x08; node->inst_size = 1; return;
        case T_I_nopc:  node->opcode = 0x0c; node->inst_size = 1; return;
        case T_I_zzazz: node->opcode = 0x07; node->inst_size = 1; return;
        case T_I_illegal: node->opcode = 0xff; node->inst_size = 1; return;

        case T_I_j_l: node->opcode = 0x9a; node->inst_size = 3; return;
        case T_I_j_g: node->opcode = 0x9b; node->inst_size = 3; return;
        case T_I_j_z: node->opcode = 0x9c; node->inst_size = 3; return;
        case T_I_j_n: node->opcode = 0x9d; node->inst_size = 3; return;
        case T_I_call_l: node->opcode = 0x9e; node->inst_size = 3; return;
        case T_I_call_g: node->opcode = 0x9f; node->inst_size = 3; return;
        case T_I_call_z: node->opcode = 0xa0; node->inst_size = 3; return;
        case T_I_call_n: node->opcode = 0xa1; node->inst_size = 3; return;

        case T_I_sys: node->opcode = 0x06; node->inst_size = 2; return;

        case T_I_lls: 
            switch(node->reg) {
                case R0: node->opcode = 0x0e; node->inst_size = 1; return;
                default: assert(false);
            } return;
        case T_I_rls: 
            switch(node->reg) {
                case R0: node->opcode = 0x0f; node->inst_size = 1; return;
                default: assert(false);
            } return;
        case T_I_swap: 
            switch(node->reg) {
                case R0: node->opcode = 0xdc; node->inst_size = 1; return;
                default: assert(false);
            } return;

        case T_I_jump: calculate_opcode_and_size_jump(node); break;
        case T_I_call: calculate_opcode_and_size_call(node); break; 
        case T_I_cmp: calculate_opcode_and_size_cmp(node); break;
        case T_I_and: calculate_opcode_and_size_and(node); break;
        case T_I_or: calculate_opcode_and_size_or(node); break;
        case T_I_xor: calculate_opcode_and_size_xor(node); break;
        case T_I_push: calculate_opcode_and_size_push(node); break;
        case T_I_pop: calculate_opcode_and_size_pop(node); break;
        case T_I_mul: calculate_opcode_and_size_mul(node); break;
        case T_I_div: calculate_opcode_and_size_div(node); break;
        case T_I_inc: calculate_opcode_and_size_inc(node); break;
        case T_I_dec: calculate_opcode_and_size_dec(node); break;
        case T_I_add: calculate_opcode_and_size_add(node); break;
        case T_I_mov: calculate_opcode_and_size_mov(node); break;
        case T_I_ld: calculate_opcode_and_size_ld(node); break;
        case T_I_ldb: calculate_opcode_and_size_ldb(node); break;
        case T_I_st: calculate_opcode_and_size_st(node); break;
        case T_I_stb: calculate_opcode_and_size_stb(node); break;
        default: assert(false);
    }
}

void addInstruction(int inst, uint16_t reg, uint16_t regm, char* label, char* text, char* define, char* imm, Token token) {
    Instruction* node = &instructions;
    while(node->instruction != T_Unknown) node = node->next;

    node->instruction = inst;
    node->reg = reg;
    node->regm = regm;
    node->label = NULL;
    node->text = NULL;
    node->define = NULL;
    node->imm = NULL;
    node->expression = token;

    if(label) node->label = strdup(label);
    if(text) node->text = strdup(text);
    if(define) node->define = strdup(define);
    if(imm) node->imm = strdup(imm);

    node->next = malloc(sizeof(Instruction));
    node->next->instruction = T_Unknown;
    node->next->next = NULL;

    calculate_opcode_and_size(node);

    //printf("Added inst with opcode %x and size %d\n", node->opcode, node->inst_size);
}

void calculate_end_of_code() {
    end_of_code = entry_point;
    Instruction* node = &instructions;
    while(node->instruction != T_Unknown) {
        end_of_code += node->inst_size;
        node = node->next;
    }
    end_of_code += get_implace_size_up_to(end_of_code);
}

uint16_t calculate_current_pos_code() {
    uint16_t pc = entry_point;
    Instruction* node = &instructions;
    while(node->instruction != T_Unknown) {
        pc += node->inst_size;
        node = node->next;
    }
    pc += get_implace_size_up_to(0xffff);
    return pc;
}

void dump_code() {
    calculate_end_of_code();
    reserve_text(end_of_code);

    uint16_t pc = entry_point;

    Instruction* node = &instructions;
    while(node->instruction != T_Unknown) {

        //printf("PC-> %04x\n", pc);

        uint16_t size = 0;
        do {
            size = find_dump_implace(pc);
            pc += size;
        } while(size != 0);

        pc += node->inst_size;

        fwrite(&(node->opcode), 1, 1 , output);

        if(node->label || node->define || node->text || node->imm || node->expression.type == T_expression) {
            uint16_t data;
            if (node->label) data = find_label(node->label);
            else if (node->define) data = find_define(node->define);
            else if (node->text) data = find_text(node->text);
            else if (node->imm) data = strtol(&(node->imm[1]), NULL, 16);
            else if (node->expression.type == T_expression) {
                Token* a = node->expression.a;
                Token* b = node->expression.b;
                Token* op = node->expression.op;

                uint16_t value_a;
                uint16_t value_b;

                switch(a->type) {
                    case T_Label: value_a = find_label(a->data); break;
                    case T_Define: value_a = find_define(a->data); break;
                    case T_TextID: value_a = find_text(a->data); break;
                    case T_Num: value_a = strtol(&(a->data[1]), NULL, 16); break;
                    default: assert(false);
                }

                switch(b->type) {
                    case T_Label: value_b = find_label(b->data); break;
                    case T_Define: value_b = find_define(b->data); break;
                    case T_TextID: value_b = find_text(b->data); break;
                    case T_Num: value_b = strtol(&(b->data[1]), NULL, 16); break;
                    default: assert(false);
                }

                switch(op->type) {
                    case T_add: data = value_a + value_b; break;
                    case T_sub: data = value_a - value_b; break;
                    default: assert(false);
                }

                free(a);
                free(b);
                free(op);
            }

            if (node->instruction == T_I_sys) fwrite(&data, 1, 1 , output); // Syscalls only have 1 byte
            else fwrite(&data, 2, 1 , output);
        }

        node = node->next;
    }

    printf("Dumping remaining implaces\n");
    for (uint16_t i = pc; i < 0xffff; ++i) {
        i += find_dump_implace(i);
    }

    printf("Code %04x -> %04x\n", entry_point, end_of_code);
}