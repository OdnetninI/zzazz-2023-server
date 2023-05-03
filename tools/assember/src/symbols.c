#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "symbols.h"
#include "file.h"

struct labels {
    char* name;
    uint16_t addr;
    struct labels* next;
};
struct labels labels = { .name = NULL, .addr = 0x0000, .next = NULL };

void add_label(const char* name, uint16_t pc) {
    struct labels* node = &labels;
    while(node->name != NULL) node = node->next;
    
    node->name = strdup(name);
    node->addr = pc;
    node->next = malloc(sizeof(struct labels));
    node->next->name = NULL;

    printf("Label %s found at position %04x\n", node->name, node->addr);
}

uint16_t find_label(const char* id) {
    struct labels* node = &labels;
    while(node->name != NULL && strcmp(node->name, id) != 0) { node = node->next; }
    //printf("Looking for %s. Found %s at %04x\n", id, node->name, node->addr);
    return node->addr;
}

struct defines {
    char* name;
    uint16_t value;
    struct defines* next;
};
struct defines defines = { .name = NULL, .value = 0x0000, .next = NULL };

void add_define(const char* name, uint16_t value) {
    struct defines* node = &defines;
    while(node->name != NULL) node = node->next;
    
    node->name = strdup(name);
    node->value = value;
    node->next = malloc(sizeof(struct defines));
    node->next->name = NULL;

    printf("Define %s found -> %04x\n", node->name, node->value);
}

uint16_t find_define(const char* id) {
    struct defines* node = &defines;
    while(node->name != NULL && strcmp(node->name, id) != 0) { node = node->next; }
    return node->value;
}

struct texts {
    char* name;
    char* text;
    int size;
    uint16_t addr;
    struct texts* next;
};

struct texts texts = { .name = NULL, .text = NULL, .next = NULL };

void add_text(const char* name, const char* text, int size) {
    struct texts* node = &texts;
    while(node->name != NULL) node = node->next;
    
    node->name = strdup(name);
    node->text = malloc(size);
    node->size = size;
    node->next = malloc(sizeof(struct texts));
    node->addr = 0x0000;

    for(int i = 0; i < size; ++i) node->text[i] = text[i];

    node->next->name = NULL;
}

void reserve_text(uint16_t eoc) {
    struct texts* node = &texts;
    while(node->name != NULL) {
        node->addr = eoc;
        eoc += node->size + 1;

        printf("Text %s has size %d located at %04x\n", node->name, node->size, node->addr);
        
        node = node->next;
    }
}

uint16_t find_text(const char* id) {
    struct texts* node = &texts;
    while(node->name != NULL && strcmp(node->name, id) != 0) { node = node->next; }
    return node->addr;
}

void dump_text() {
    struct texts* node = &texts;
    while(node->name != NULL) {
        node->text[node->size] = 0x00;
        fwrite(node->text, 1, node->size + 1, output);
        node = node->next;
    }
}