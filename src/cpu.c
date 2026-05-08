#include "cpu.h"

Registers init_registers() {
    Registers registers = {0, 0, 0, 0, 0, 0, 0, 0, 0xFFFE};
    return registers;
}

uint16_t get_af(Registers registers) {
    return registers.a << 8 | registers.f;
}

uint16_t get_bc(Registers registers) {
    return registers.b << 8 | registers.c;
}

uint16_t get_de(Registers registers) {
    return registers.d << 8 | registers.e;
}

uint16_t get_hl(Registers registers) {
    return registers.h << 8 | registers.l;
}

void set_af(Registers registers, uint16_t value) {
    registers.a = ((value & 0xFF00) >> 8);
    registers.f = (value & 0xFF);
}

void set_bc(Registers registers, uint16_t value) {
    registers.b = ((value & 0xFF00) >> 8);
    registers.c = (value & 0xFF);
}

void set_de(Registers registers, uint16_t value) {
    registers.d = ((value & 0xFF00) >> 8);
    registers.e = (value & 0xFF);
}

void set_hl(Registers registers, uint16_t value) {
    registers.h = ((value & 0xFF00) >> 8);
    registers.l = (value & 0xFF);
}

Cpu init_cpu() {
    Cpu cpu = {
        .pc = 0x100,
        .memory = {0},
        .cycles = 0
    };
    return cpu;
}