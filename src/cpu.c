#include <stddef.h>
#include "cpu.h"

enum { NUM_OPCODES = 256 };
enum { MEMORY_SIZE = 0xFFFF };

enum {
    ZERO_FLAG_BYTE_POSITION = 7,
    SUBTRACT_FLAG_BYTE_POSITION = 6,
    HALF_CARRY_FLAG_BYTE_POSITION = 5,
    CARRY_FLAG_BYTE_POSITION = 4
};

typedef enum {
    REG_A,
    REG_B,
    REG_C,
    REG_D,
    REG_E,
    REG_H,
    REG_L,
} SingleRegister;

typedef enum  {
    REG_AF,
    REG_BC,
    REG_DE,
    REG_HL,
    REG_SP,
} RegisterPair;

typedef void (*InstructionFunc)(Cpu*, int);

typedef struct {
    char* name;
    InstructionFunc handler;
    int arg;
} Instruction;

// Prototypes
static void nop();
static void ld_rr_nn(Cpu* cpu, RegisterPair rr);

static void inc_r(Cpu* cpu, SingleRegister target);
static void dec_r(Cpu* cpu, SingleRegister target);

static void rlca(Cpu* cpu);


Instruction opcodes_8bit[NUM_OPCODES] = {
    [0x00] = {"NOP", nop, NULL},
    [0x01] = {"LD BC, nn", ld_rr_nn, REG_BC},

    [0x04] = {"INC B", inc_r, REG_B},
    [0x05] = {"DEC B", dec_r, REG_B},

    [0x07] = {"RLCA", rlca, NULL},

    [0x11] = {"LD DE, nn", ld_rr_nn, REG_DE},

    [0x14] = {"INC D", inc_r, REG_D},
    [0x15] = {"INC H", inc_r, REG_H},
};

Instruction opcodes_16bit[NUM_OPCODES] = {

};

Registers init_registers() {
    FlagsRegister flags = {false};
    Registers registers = {0, flags, 0, 0, 0, 0, 0, 0};
    return registers;
}

uint16_t get_bc(Registers registers) {
    return registers.b << 8 | registers.c;
}

static void set_rr(Registers* registers, RegisterPair rr, uint16_t value) {
    uint8_t high = ((value & 0xFF00) >> 8);
    uint8_t low = (value & 0xFF);

    uint8_t* reg1 = NULL;
    uint8_t* reg2 = NULL;

    switch (rr)
    {
    case REG_AF:
        reg1 = &registers->a;
        reg2 = &registers->f;
        break;
    case REG_BC:
        reg1 = &registers->b;
        reg2 = &registers->c;
        break;
    case REG_DE:
        reg1 = &registers->d;
        reg2 = &registers->e;
        break;
    case REG_HL:
        reg1 = &registers->h;
        reg2 = &registers->l;
        break;
    default:
        break; // TODO: handle error
    }
    set_r(registers, reg1, high);
    set_r(registers, reg2, low);
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

uint8_t get_register_value(Registers registers, SingleRegister target) {
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

static void set_r(Registers* registers, SingleRegister target, uint8_t value) {
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

static void set_register_pair(Registers* registers, RegisterPair rr, uint16_t nn) {
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
        .sp = NULL, // TODO, set to correct value
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

static void execute_not_prefixed(Cpu* cpu, uint8_t opcode) {
    Instruction instr = opcodes_8bit[opcode];

    if (instr.handler != NULL) 
    {
        instr.handler(cpu, instr.arg);
    }
}

static void execute_prefixed(Cpu* cpu, uint8_t opcode) {
    Instruction instr = opcodes_16bit[opcode];

    if (instr.handler != NULL) 
    {
        instr.handler(cpu, instr.arg);
    }
}

static void tick(Cpu* cpu) {
    cpu->t_cycles += 4;
}

uint8_t bus_read(Cpu* cpu, uint16_t address) {
    //if (address <= )
    // TODO: Implement bus_read
}

uint8_t read_memory(Cpu* cpu, uint16_t address) {
    tick(cpu);
    return cpu->bus.memory[(size_t) address]; // TODO switch to bus_read
}

uint8_t get_opcode(Cpu* cpu) {
    uint8_t opcode = read_memory(cpu, cpu->pc);
    cpu->pc++;
    return opcode;
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


// #################
// # 8-bit opcodes #
// #################
static void nop() {}

static void ld_rr_nn(Cpu* cpu, RegisterPair rr) {
    uint16_t nn = read_nn(cpu);
    //TODO

}

static void inc_r(Cpu* cpu, SingleRegister target) {
    uint8_t val = get_register_value(cpu->registers, target);
    uint8_t new_val = val + 1;

    Registers* registers = &cpu->registers;

    registers->f.zero = new_val == 0,
    registers->f.subtract = false;
    registers->f.carry = unsigned_overflow(val, 1);
    registers->f.half_carry = (val & 0xF) + (1 & 0xF) > 0xF;

    set_r(registers, target, new_val);
}

static void dec_r(Cpu* cpu, SingleRegister target) {
    uint8_t val = get_register_value(cpu->registers, target);
    uint8_t new_val = val - 1;

    Registers* registers = &cpu->registers;

    registers->f.zero = new_val == 0,
    registers->f.subtract = true;
    registers->f.half_carry = (val & 0xF) + (1 & 0xF) > 0xF; //TODO, fix

    set_r(registers, target, new_val);
}

static void rlca(Cpu* cpu) {
    uint8_t val = get_register_value(cpu->registers, REG_A);
    uint8_t carry = (val >> 7);
    val = (val << 1) | carry;

    cpu->registers.f.zero = false;
    cpu->registers.f.subtract = false;
    cpu->registers.f.half_carry = false;
    cpu->registers.f.carry = carry;

    set_r(&cpu->registers, REG_A, val);
}

static void add_r(Cpu* cpu, SingleRegister target) {
    Registers* registers = &cpu->registers;
    uint8_t val1 = get_register_value(*registers, REG_A);
    uint8_t val2 = get_register_value(*registers, target);

    uint16_t sum = val1 + val2;

    registers->f.zero = sum == 0,
    registers->f.subtract = false;
    registers->f.carry = unsigned_overflow(val1, val2);
    registers->f.half_carry = (val1 & 0xF) + (val2 & 0xF) > 0xF;

    set_r(registers, REG_A, sum);
}

// ##################
// # 16-bit opcodes #
// ##################
static void rlc_r(Cpu* cpu, SingleRegister target) {
    uint8_t reg = get_register_value(cpu->registers, target);
    uint8_t carry = (reg >> 7);
    reg = (reg << 1) | carry;

    cpu->registers.f.zero = carry;

    set_r(&cpu->registers, target, reg);
}