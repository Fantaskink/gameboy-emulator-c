#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stddef.h>
#include <builtins.h>
#include <stdbool.h>

#define NUM_OPCODES 256
#define MEMORY_SIZE 0xFFFF

#define ZERO_FLAG_BYTE_POSITION 7
#define SUBTRACT_FLAG_BYTE_POSITION 6
#define HALF_CARRY_FLAG_BYTE_POSITION 5
#define CARRY_FLAG_BYTE_POSITION 4

Cpu init_cpu();
void step(Cpu* cpu);

typedef struct {
    bool zero;
    bool subtract;
    bool half_carry;
    bool carry;
} FlagsRegister;

typedef struct {
    uint8_t a;
    FlagsRegister f;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;
} Registers;

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
    uint8_t memory[MEMORY_SIZE];
} MemoryBus;

typedef struct {
    Registers registers;
    uint16_t pc;
    uint16_t sp;
    MemoryBus bus;
    uint64_t t_cycles;
} Cpu;

typedef void (*InstructionFunc)(Cpu*, int);

typedef struct {
    char* name;
    InstructionFunc handler;
    int arg;
} Instruction;

#endif