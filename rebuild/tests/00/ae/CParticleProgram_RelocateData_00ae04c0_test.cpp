// Behaviour fixture for CParticleProgram::RelocateData (provider CParticleProgram_RelocateData_00ae04c0.cpp), called
// non-virtually on raw storage: NonSimPositions (+0xC0) moves by (new - old) keeping its offset; null stays null; nothing
// else changes.
#include <stdio.h>
#include <string.h>
#include "fable_particle_program.h"

int main()
{
    static unsigned char raw[0x140], before[0x140];
    static unsigned char oldBlock[64], newBlock[64];
    for (int i = 0; i < 0x140; ++i) raw[i] = (unsigned char)i;
    void* p = oldBlock + 48; memcpy(raw + 0xC0, &p, 4);
    memcpy(before, raw, sizeof(raw));
    CParticleProgram* program = reinterpret_cast<CParticleProgram*>(raw);
    program->CParticleProgram::RelocateData(1, oldBlock, newBlock, 64);
    void* q; memcpy(&q, raw + 0xC0, 4);
    bool ok = q == newBlock + 48 && memcmp(raw, before, 0xC0) == 0 && memcmp(raw + 0xC4, before + 0xC4, 0x140 - 0xC4) == 0;
    void* z = 0; memcpy(raw + 0xC0, &z, 4);
    program->CParticleProgram::RelocateData(1, oldBlock, newBlock, 64);
    memcpy(&q, raw + 0xC0, 4);
    ok = ok && q == 0;
    if (!ok) { printf("PROGRAM_RELOCATE_FAIL\n"); return 1; }
    printf("PROGRAM_RELOCATE_OK\n");
    return 0;
}
