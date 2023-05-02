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

uint8_t memory[0xffff] = {0};

uint8_t mem_read(uint16_t addr) {
    if (addr < 0x1000 && cpu.PC >= 0x1000) return 0x55;
    return memory[addr];
}

void mem_write(uint16_t addr, uint8_t data) {
    if (addr < 0x1000 && cpu.PC >= 0x1000) return;
    memory[addr] = data;
}

uint16_t mem_read_16(uint16_t addr) {
    uint8_t low = mem_read(addr);
    uint8_t high = mem_read(addr + 1);
    return ((uint16_t)high << 8) | ((uint16_t)low);
}

void mem_write_16(uint16_t addr, uint16_t data) {
    uint8_t low = data & 0xff;
    uint16_t high = (data >> 8) & 0xff;
    mem_write(addr, low);
    mem_write(addr + 1, high);
}