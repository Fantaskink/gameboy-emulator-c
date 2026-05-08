#include "cpu.h"

Registers init_registers() {
    Registers registers = {0, 0, 0, 0, 0, 0, 0, 0, 0xFFFE};
    return registers;
}
/*
uint16_t get_af(Registers registers) {
    return registers.a << 8 | registers.f;
}
*/

uint16_t get_bc(Registers registers) {
    return registers.b << 8 | registers.c;
}

uint16_t get_de(Registers registers) {
    return registers.d << 8 | registers.e;
}

uint16_t get_hl(Registers registers) {
    return registers.h << 8 | registers.l;
}
/*
void set_af(Registers registers, uint16_t value) {
    registers.a = ((value & 0xFF00) >> 8);
    registers.f = (value & 0xFF);
}
*/

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

uint8_t flags_to_byte(FlagsRegister flag) {
    uint8_t out = 0;
    out = out | flag.zero << ZERO_FLAG_BYTE_POSITION;
    out = out | flag.subtract << SUBTRACT_FLAG_BYTE_POSITION;
    out = out | flag.half_carry << HALF_CARRY_FLAG_BYTE_POSITION;
    out = out | flag.carry << CARRY_FLAG_BYTE_POSITION;
    return out;
}

FlagsRegister byte_to_flags(uint8_t value) {
    FlagsRegister flag;
    flag.zero =  (value & 1 << ZERO_FLAG_BYTE_POSITION) >> ZERO_FLAG_BYTE_POSITION;
    flag.subtract =  (value & 1 << SUBTRACT_FLAG_BYTE_POSITION) >> SUBTRACT_FLAG_BYTE_POSITION;
    flag.half_carry =  (value & 1 << HALF_CARRY_FLAG_BYTE_POSITION) >> HALF_CARRY_FLAG_BYTE_POSITION;
    flag.carry =  (value & 1 << CARRY_FLAG_BYTE_POSITION) >> CARRY_FLAG_BYTE_POSITION;

    return flag;
}

uint8_t get_register_value(Registers registers, ArithmeticTarget target) {
    switch (target)
    {
    case A:
        return registers.a;
    case B:
        return registers.b;
    case C:
        return registers.c;
    case D:
        return registers.d;
    case E:
        return registers.e;
    case H:
        return registers.h;
    case L:
        return registers.l;
    }
}

void set_register_value(Registers* registers, ArithmeticTarget target, uint8_t value) {
    switch (target)
    {
    case A:
        registers->a = value;
    case B:
        registers->b = value;
    case C:
        return registers->c = value;
    case D:
        return registers->d = value;
    case E:
        return registers->e = value;
    case H:
        return registers->h = value;
    case L:
        return registers->l = value;
    }
}

Cpu init_cpu() {
    Cpu cpu = {
        .pc = 0x100,
        .memory = {0},
        .cycles = 0
    };
    return cpu;
}

// Returns 1 if the operation causes an overflow, 0 if not
inline bool unsigned_overflow(uint8_t a, uint8_t b) {
    return (a+b)<a; // 1 if the result is lower than one of its operands
}

void add(Cpu cpu, ArithmeticTarget reg1, ArithmeticTarget reg2) {
    Registers registers = cpu.registers;
    uint8_t val1 = get_register_value(registers, reg1);
    uint8_t val2 = get_register_value(registers, reg2);

    registers.f.carry = unsigned_overflow(val1, val2);
    

    cpu.cycles += 1;
}