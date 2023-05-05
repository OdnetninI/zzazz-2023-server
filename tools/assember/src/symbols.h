#ifndef __SYMS_H__
#define __SYMS_H__

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

void add_label(const char* name, uint16_t pc);
uint16_t find_label(const char* id);

void add_define(const char* name, uint16_t value);
uint16_t find_define(const char* id);

void add_text(const char* name, const char* text, int size);
void reserve_text(uint16_t eoc);
uint16_t find_text(const char* id);
void dump_text();

void add_implace(uint16_t pc, const char* text, int size);
uint16_t get_implace_size_up_to(uint16_t pc);
uint16_t find_dump_implace(uint16_t pc);

#endif /* __SYMS_H__ */