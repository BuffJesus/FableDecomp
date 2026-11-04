// Behaviour fixture for CParticleSimulation::RelocateData (provider CParticleSimulation_RelocateData_00adfc60.cpp), called
// non-virtually on raw storage: each non-null particle array pointer moves by (new - old) keeping its offset in the
// block; a null array stays null; nothing else changes.
#include <stdio.h>
#include <string.h>
#include "fable_particle_simulation.h"

int main()
{
    static unsigned char raw[0x80], before[0x80];
    static unsigned char oldBlock[256], newBlock[256];
    for (int i = 0; i < 0x80; ++i) raw[i] = (unsigned char)i;
    void* p[5] = { oldBlock, oldBlock + 64, 0, oldBlock + 160, oldBlock + 240 };
    memcpy(raw + 0x1C, p, 20);
    memcpy(before, raw, sizeof(raw));
    reinterpret_cast<CParticleSimulation*>(raw)->CParticleSimulation::RelocateData(3, oldBlock, newBlock, 256);
    void* q[5]; memcpy(q, raw + 0x1C, 20);
    bool ok = q[0] == newBlock && q[1] == newBlock + 64 && q[2] == 0 && q[3] == newBlock + 160 && q[4] == newBlock + 240;
    ok = ok && memcmp(raw, before, 0x1C) == 0 && memcmp(raw + 0x30, before + 0x30, 0x50) == 0;
    if (!ok) { printf("SIMULATION_RELOCATE_FAIL\n"); return 1; }
    printf("SIMULATION_RELOCATE_OK\n");
    return 0;
}
