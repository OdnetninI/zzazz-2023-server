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