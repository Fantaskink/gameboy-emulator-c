#include <stddef.h>
#include "cpu.h"

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

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t z;
    uint16_t p;
    uint16_t q;
} OpcodeData;

// Prototypes
static void nop();
static void ld_rr_nn(Cpu* cpu, RegisterPair rr);

static void inc_r(Cpu* cpu, SingleRegister target);
static void dec_r(Cpu* cpu, SingleRegister target);

static void rlca(Cpu* cpu);

SingleRegister r_table[8] = {
    REG_A,
    REG_B,
    REG_C,
    REG_D,
    REG_E,
    REG_H,
    REG_L,
};

Registers init_registers() {
    FlagsRegister flags = {false};
    Registers registers = {0, flags, 0, 0, 0, 0, 0, 0};
    return registers;
}

uint16_t get_rr(Registers registers, RegisterPair rr) {

    uint8_t r1_val = 0;
    uint8_t r2_val = 0;

    switch (rr)
    {
    case REG_AF:
        r1_val = registers.a;
        r2_val = flags_to_byte(registers.f);
        break;
    case REG_BC:
        r1_val = registers.b;
        r2_val = registers.c;
        break;
    case REG_DE:
        r1_val = registers.d;
        r2_val = registers.e;
        break;
    case REG_HL:
        r1_val = registers.h;
        r2_val = registers.l;
        break;
    default:
        break; // TODO: handle error
    }

    return r1_val << 8 | r2_val;
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

uint8_t get_r(Registers registers, SingleRegister target) {
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

OpcodeData decode_opcode(uint8_t opcode) {
    uint32_t x = opcode & 0b11000000;
    uint32_t y = opcode & 0b00111000;
    uint32_t z = opcode & 0b00000111;
    uint32_t p = y >> 1;
    uint32_t q = y % 2;

    return (OpcodeData) {x, y, z, p, q};
}


void step(Cpu* cpu) {
    uint8_t opcode = get_opcode(cpu);
    bool is_prefixed = opcode == 0xCB;

    if (is_prefixed)
    {
        opcode = get_opcode(cpu);
        OpcodeData data = decode_opcode(opcode);
        execute_prefixed(cpu, data);
    } else{
        OpcodeData data = decode_opcode(opcode);
        execute_not_prefixed(cpu, data);
    }
}

static void execute_not_prefixed(Cpu* cpu, OpcodeData data) {
    switch (data.x)
    {
    case 0:
        /* code */
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    default:
        break;
    }
}

void match_z(Cpu* cpu, OpcodeData data) {
    switch (data.z)
    {
    case 0:
        break;
    case 4:
        inc_r(cpu, r_table[data.y]);
        break;
    case 5:
        dec_r(cpu, r_table[data.y]);
        break;
    case 6:
        ld_r_n(cpu, r_table[data.y]);
        break;
    
    default:
        break;
    }
}

static void execute_prefixed(Cpu* cpu, OpcodeData data) {
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

uint8_t read_n(Cpu* cpu) {
    uint8_t n = read_memory(cpu, cpu->pc);
    cpu->pc++;
    return n;
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

    set_rr(&cpu->registers, rr, nn);
}

static void inc_rr(Cpu* cpu, RegisterPair rr) {
    uint16_t nn = get_rr(cpu->registers, rr) + 1;

    set_rr(&cpu->registers, rr, nn);
}

static void inc_r(Cpu* cpu, SingleRegister target) {
    uint8_t val = get_r(cpu->registers, target);
    uint8_t new_val = val + 1;

    Registers* registers = &cpu->registers;

    registers->f.zero = new_val == 0,
    registers->f.subtract = false;
    registers->f.carry = unsigned_overflow(val, 1);
    registers->f.half_carry = (val & 0xF) + (1 & 0xF) > 0xF;

    set_r(registers, target, new_val);
}

static void dec_r(Cpu* cpu, SingleRegister target) {
    uint8_t val = get_r(cpu->registers, target);
    uint8_t new_val = val - 1;

    Registers* registers = &cpu->registers;

    registers->f.zero = new_val == 0,
    registers->f.subtract = true;
    registers->f.half_carry = (val & 0xF) + (1 & 0xF) > 0xF; //TODO, fix

    set_r(registers, target, new_val);
}

static void ld_r_n(Cpu* cpu, SingleRegister target) { // Load the immediate 8 bits into register r
    uint8_t val = read_n(cpu);
    set_r(&cpu->registers, target, val);
}

static void rlca(Cpu* cpu) {
    uint8_t val = get_r(cpu->registers, REG_A);
    uint8_t carry = (val >> 7);
    val = (val << 1) | carry;

    cpu->registers.f.zero = false;
    cpu->registers.f.subtract = false;
    cpu->registers.f.half_carry = false;
    cpu->registers.f.carry = carry;

    set_r(&cpu->registers, REG_A, val);
}

static void add_rr_rr(Cpu* cpu, RegisterPair dst, RegisterPair src) {
    uint16_t val1 = get_rr(cpu->registers, dst);
    uint16_t val2 = get_rr(cpu->registers, src);

    
}

static void add_r(Cpu* cpu, SingleRegister target) {
    Registers* registers = &cpu->registers;
    uint8_t val1 = get_r(*registers, REG_A);
    uint8_t val2 = get_r(*registers, target);

    uint16_t sum = val1 + val2;

    registers->f.zero = sum == 0,
    registers->f.subtract = false;
    registers->f.carry = unsigned_overflow(val1, val2);
    registers->f.half_carry = (val1 & 0xF) + (val2 & 0xF) > 0xF;

    set_r(registers, REG_A, sum);
}

static void ld_r_r(Cpu* cpu, SingleRegister dst, SingleRegister src) {
    uint8_t val = get_r(cpu->registers, src);
    set_r(&cpu->registers, dst, val);
}

// ##################
// # 16-bit opcodes #
// ##################
static void rlc_r(Cpu* cpu, SingleRegister target) {
    uint8_t reg = get_r(cpu->registers, target);
    uint8_t carry = (reg >> 7);
    reg = (reg << 1) | carry;

    cpu->registers.f.zero = carry;

    set_r(&cpu->registers, target, reg);
}

static void res_b_r(Cpu* cpu, uint8_t bit_pos, SingleRegister r) {
    uint8_t val = get_r(cpu->registers, r);
    val = val | 0 << bit_pos;
    set_r(&cpu->registers, r, val);
}

static void res_b_addr(Cpu* cpu, uint8_t b, RegisterPair rr) {
    uint16_t addr = get_rr(cpu->registers, rr);


}