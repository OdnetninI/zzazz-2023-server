#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define FILE_IMP
#include "file.h"

#include "lex.h"
#include "gram.h"
#include "code.h"
#include "symbols.h"

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

    parse();
    dump_code();
    dump_text();
    
    fclose(input);
    fclose(output);
    return 0;
}