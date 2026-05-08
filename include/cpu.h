#ifndef CPU.H
#define CPU.H

#include <stdint.h>
#include <stdbool.h>

#define ZERO_FLAG_BYTE_POSITION 7
#define SUBTRACT_FLAG_BYTE_POSITION 6
#define HALF_CARRY_FLAG_BYTE_POSITION 5
#define CARRY_FLAG_BYTE_POSITION 4


typedef struct {
    uint8_t a;
    uint8_t f;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;
    uint16_t sp;

} Registers;

typedef struct {
    bool zero;
    bool subtraction;
    bool half_carry;
    bool carry;
} FlagsRegister;

typedef enum {
    A,
    B,
    C,
    D,
    E,
    H,
    L,
} SingleRegister;

typedef enum  {
    AF,
    BC,
    DE,
    HL,
    SP,
} DoubleRegister;

typedef struct {
    uint16_t pc;
    uint8_t memory[0x10000];
    uint64_t cycles;
} Cpu;

#endif