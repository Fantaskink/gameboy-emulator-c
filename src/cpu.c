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
    case REG_A:
        return registers.a;
    case REG_B:
        return registers.b;
    case REG_C:
        return registers.c;
    case REG_D:
        return registers.d;
    case REG_E:
        return registers.e;
    case REG_H:
        return registers.h;
    case REG_L:
        return registers.l;
    }
}

void set_register_value(Registers* registers, ArithmeticTarget target, uint8_t value) {
    switch (target)
    {
    case REG_A:
        registers->a = value;
        break;
    case REG_B:
        registers->b = value;
        break;
    case REG_C:
        registers->c = value;
        break;
    case REG_D:
        registers->d = value;
        break;
    case REG_E:
        registers->e = value;
        break;
    case REG_H:
        registers->h = value;
        break;
    case REG_L:
        registers->l = value;
        break;
    }
}

void set_double_register_value(Registers* registers, RegisterPair rr, uint16_t nn) {
    switch (rr)
    {
    case REG_AF:
        
        break;
    
    default:
        break;
    }
}

MemoryBus init_bus() {
    MemoryBus bus;
    return bus;
}

Cpu init_cpu() {
    Cpu cpu = {
        .registers = init_registers(),
        .pc = 0x100,
        .bus = init_bus(),
        .t_cycles = 0
    };
    return cpu;
}

void step(Cpu* cpu) {
    uint8_t opcode = get_opcode(cpu);
    bool is_prefixed = opcode == 0xCB;

    if (is_prefixed)
    {
        opcode = get_opcode(cpu);
        execute_prefixed(cpu, opcode);
    } else{
        execute_not_prefixed(cpu, opcode);
    }
}

void execute_not_prefixed(Cpu* cpu, uint8_t opcode) {
    Instruction instr = opcodes_8bit[opcode];

    if (instr.handler != NULL) 
    {
        instr.handler(cpu, instr.arg);
    }
}

void execute_prefixed(Cpu* cpu, uint8_t opcode) {
    Instruction instr = opcodes_16bit[opcode];

    if (instr.handler != NULL) 
    {
        instr.handler(cpu, instr.arg);
    }
}

// Returns 1 if the operation causes an overflow; 0 if not
inline bool unsigned_overflow(uint8_t a, uint8_t b) {
    return (a+b)<a; // 1 if the result is lower than one of its operands
}

void tick(Cpu* cpu) {
    cpu->t_cycles += 4;
}

uint8_t bus_read(Cpu* cpu, uint16_t address) {
    //if (address <= )
}

uint8_t read_memory(Cpu* cpu, uint16_t address) {
    tick(cpu);
    return cpu->bus.memory[(size_t) address]; // TODO switch to bus_read
}

uint8_t get_opcode(Cpu* cpu) {
    uint8_t opcode = read_memory(cpu, cpu->pc);
    cpu->pc++;
}

uint16_t bytes_to_uint16(uint8_t high, uint8_t low) {
    return ((uint16_t)high << 8) | low;
}

uint16_t read_nn(Cpu* cpu) {
    uint8_t nn_lsb = read_memory(cpu, cpu->pc);
    cpu->pc++;
    uint8_t nn_msb = read_memory(cpu, cpu->pc);
    cpu->pc++;
    uint16_t nn = bytes_to_uint16(nn_msb, nn_lsb);
    return nn;
}

void nop(Cpu* cpu) {
    cpu->t_cycles += 4;
}

void ld_rr_nn(Cpu* cpu, RegisterPair rr) {
    uint16_t nn = read_nn(cpu);
    //TODO

}

// ##################
// # 16-bit opcodes #
// ##################
void rlc_r(Cpu* cpu, ArithmeticTarget target) {
    uint8_t reg = get_register_value(cpu->registers, target);
    uint8_t carry = (reg >> 7);
    reg = (reg << 1) | carry;

    cpu->registers.f.zero = carry;

    set_register_value(&cpu->registers, target, reg);
}

void add_r(Cpu* cpu, ArithmeticTarget reg1, ArithmeticTarget reg2) {
    Registers* registers = &cpu->registers;
    uint8_t val1 = get_register_value(*registers, reg1);
    uint8_t val2 = get_register_value(*registers, reg2);

    uint8_t new_val = val1 + val2;

    registers->f.zero = new_val == 0,
    registers->f.subtract = false;
    registers->f.carry = unsigned_overflow(val1, val2);
    registers->f.half_carry = (val1 & 0xF) + (val2 & 0xF) > 0xF;

    set_register_value(registers, reg1, new_val);
}