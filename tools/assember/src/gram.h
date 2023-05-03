#ifndef __GRAM_H__
#define __GRAM_H__

#pragma once

enum Register {
    R_Unknown,
    PC,SP,
    R0,R1,R2,R3,
    Num_Registers
};

void parse();

#endif /* __GRAM_H__ */