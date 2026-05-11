#include <stdio.h>

#include "cpu.h"

int main(int argc, char const *argv[])
{
    Cpu cpu = init_cpu();

    while (true)
    {
        step(&cpu);
    }
    
    return 0;
}
