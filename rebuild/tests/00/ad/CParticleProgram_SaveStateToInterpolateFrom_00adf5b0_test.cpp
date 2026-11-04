// Behaviour fixture for CParticleProgram::SaveStateToInterpolateFrom (provider
// CParticleProgram_SaveStateToInterpolateFrom_00adf5b0.cpp). The program and the simulation are raw storage at retail
// offsets (simulation Size +0x18, Positions +0x1C, LastPositions +0x20, SavedPositions +0x24). Checked: exactly Size four-component
// positions (16 bytes each) are copied from Positions to SavedPositions, the bytes after them, the source and
// LastPositions are left alone, the program is not touched, and a zero Size copies nothing.
#include <stdio.h>
#include <string.h>
#include "fable_particle_program.h"

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

static void Put(unsigned char* raw, unsigned offset, const void* value) { memcpy(raw + offset, &value, 4); }

int main()
{
    static unsigned char program[0x140], programBefore[0x140], sim[0x80], simBefore[0x80];
    static unsigned char positions[5 * 16], saved[5 * 16], positionsBefore[5 * 16], last[5 * 16];
    unsigned long size, i;
    memset(program, 0x5A, sizeof(program)); memcpy(programBefore, program, sizeof(program));
    for (i = 0; i < sizeof(positions); ++i) positions[i] = (unsigned char)(i * 7 + 1);
    memcpy(positionsBefore, positions, sizeof(positions));
    memset(saved, 0xEE, sizeof(saved));
    memset(last, 0x77, sizeof(last));
    memset(sim, 0xCD, sizeof(sim));
    size = 3; memcpy(sim + 0x18, &size, 4);
    Put(sim, 0x1C, positions); Put(sim, 0x20, last); Put(sim, 0x24, saved);
    memcpy(simBefore, sim, sizeof(sim));

    CParticleProgram* p = reinterpret_cast<CParticleProgram*>(program);
    p->SaveStateToInterpolateFrom(*reinterpret_cast<CParticleSimulation*>(sim));
    CHECK(memcmp(saved, positions, 3 * 16) == 0);
    for (i = 3 * 16; i < sizeof(saved); ++i) CHECK(saved[i] == 0xEE);
    CHECK(memcmp(positions, positionsBefore, sizeof(positions)) == 0);
    for (i = 0; i < sizeof(last); ++i) CHECK(last[i] == 0x77);
    CHECK(memcmp(sim, simBefore, sizeof(sim)) == 0);
    CHECK(memcmp(program, programBefore, sizeof(program)) == 0);

    // Zero particles: nothing copied.
    memset(saved, 0xEE, sizeof(saved));
    size = 0; memcpy(sim + 0x18, &size, 4);
    p->SaveStateToInterpolateFrom(*reinterpret_cast<CParticleSimulation*>(sim));
    for (i = 0; i < sizeof(saved); ++i) CHECK(saved[i] == 0xEE);

    if (g_fail) { printf("SAVE_STATE_FAIL %d\n", g_fail); return 1; }
    printf("SAVE_STATE_OK\n");
    return 0;
}
