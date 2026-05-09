#ifndef CPU.H
#define CPU.H

#include <stdint.h>
#include <stddef.h>
#include <builtins.h>
#include <stdbool.h>

#define ZERO_FLAG_BYTE_POSITION 7
#define SUBTRACT_FLAG_BYTE_POSITION 6
#define HALF_CARRY_FLAG_BYTE_POSITION 5
#define CARRY_FLAG_BYTE_POSITION 4


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
    uint16_t sp;
} Registers;

typedef enum {
    REG_A,
    REG_B,
    REG_C,
    REG_D,
    REG_E,
    REG_H,
    REG_L,
} ArithmeticTarget;

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

#define NUM_OPCODES 256

Instruction opcodes_8bit[NUM_OPCODES] = {
    [0x01] = {"LD BC, nn", ld_rr_nn, REG_BC},
    [0x11] = {"LD DE, nn", ld_rr_nn, REG_DE},
};

Instruction opcodes_16bit[NUM_OPCODES] = {

};
 

typedef struct {
    uint8_t memory[0xFFFF];
} MemoryBus;

typedef struct {
    Registers registers;
    uint16_t pc;
    MemoryBus bus;
    uint64_t t_cycles;
} Cpu;

#endif