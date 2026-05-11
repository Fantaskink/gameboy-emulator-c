#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>

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

#endif