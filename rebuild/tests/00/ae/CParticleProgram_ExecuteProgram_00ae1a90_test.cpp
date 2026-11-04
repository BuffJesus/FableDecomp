// Behaviour fixture for CParticleProgram::ExecuteProgram and ApplyConstraints (provider
// CParticleProgram_ExecuteProgram_00ae1a90.cpp), reached through their mangled names (private). The program is raw
// storage at retail offsets (Constraints stream +0x114, LoopStack +0x12C); the fixture supplies the stream's PeekData /
// GetLength over a hand-built record stream and SatisfyDistanceConstraints (recording payload pointer and count; it also
// sets the simulation's MaximumAdjustment +0x30 from a script). Checked: a stale loop stack is cleared first; records are
// walked type / count / size + payload; REPEAT n repeats the records up to its END_REPEAT n times (nested loops too);
// DISTANCE passes its payload and count; MIN_MAX_DISTANCE, UNBEND and SPHERE_COLLISION records (empty solvers) and
// unknown types are skipped by their size; an END_REPEAT with no open loop is ignored; the loop stack ends empty.
// ApplyConstraints resets MaximumAdjustment before every pass and stops after the first pass whose adjustment is <= the
// error limit (equal counts as converged), or after maximumIterations passes; zero iterations runs nothing.
#include <stdio.h>
#include <string.h>
#include "fable_particle_program.h"

static int g_fail, g_calls, g_seen0Calls;
static unsigned long g_stream[64]; static unsigned long g_length;
static const void* g_payload[64]; static unsigned long g_counts[64];
static float g_script[8]; static int g_scriptLen, g_pass; static float g_resetSeen[8];
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

const void* CMemoryDataOutputStream::PeekData() const { return g_stream; }
unsigned long CMemoryDataOutputStream::GetLength() const { return g_length; }
void CParticleSimulation::SatisfyDistanceConstraints(const CParticleDistanceConstraint* c, unsigned long count)
{
    if (g_calls < 64) { g_payload[g_calls] = c; g_counts[g_calls] = count; }
    ++g_calls;
    if (g_scriptLen) { float a; memcpy(&a, reinterpret_cast<unsigned char*>(this) + 0x30, 4); if (g_pass < 8) g_resetSeen[g_pass] = a;
                       float v = g_script[g_pass < g_scriptLen ? g_pass : g_scriptLen - 1]; ++g_pass; memcpy(reinterpret_cast<unsigned char*>(this) + 0x30, &v, 4); }
}

extern "C" void __fastcall FixExecute(const void* self, int, void* simulation);
extern "C" void __fastcall FixApply(const void* self, int, void* simulation, long iterations, float error);
#pragma comment(linker, "/alternatename:@FixExecute@12=?ExecuteProgram@CParticleProgram@@ABEXAAVCParticleSimulation@@@Z")
#pragma comment(linker, "/alternatename:@FixApply@20=?ApplyConstraints@CParticleProgram@@ABEXAAVCParticleSimulation@@JM@Z")

static unsigned long g_w;
static void Rec(unsigned long type, unsigned long count, unsigned long payloadWords)
{
    g_stream[g_w++] = type; g_stream[g_w++] = count; g_stream[g_w++] = payloadWords * 4;
    for (unsigned long i = 0; i < payloadWords; ++i) g_stream[g_w++] = 0xA0000000u + g_w;
}
static unsigned long LoopCount(const unsigned char* raw) { unsigned long b, e; memcpy(&b, raw + 0x12C, 4); memcpy(&e, raw + 0x130, 4); return (e - b) / 8; }

int main()
{
    static unsigned char program[0x140], sim[0x80];
    static unsigned long stale[2][2] = { { 1, 2 }, { 3, 4 } };
    unsigned long sb = (unsigned long)stale, se = sb + sizeof(stale);
    memcpy(program + 0x12C, &sb, 4); memcpy(program + 0x130, &se, 4); memcpy(program + 0x134, &se, 4);

    // REPEAT 2 { DISTANCE(2) REPEAT 3 { DISTANCE(1) MINMAX UNBEND SPHERE type9 } END } END ; END(no loop) ; DISTANCE(5)
    g_w = 0;
    Rec(0, 2, 1);   // a REPEAT payload is skipped: the loop resumes after it
    unsigned long a = g_w; Rec(2, 2, 8);
    Rec(0, 3, 0);
    unsigned long b = g_w; Rec(2, 1, 4);
    Rec(3, 7, 5); Rec(4, 7, 3); Rec(5, 7, 4); Rec(9, 7, 2);
    Rec(1, 0, 0);
    Rec(1, 0, 0);
    Rec(1, 0, 0);
    unsigned long c = g_w; Rec(2, 5, 1);
    g_length = g_w * 4;

    FixExecute(program, 0, sim);
    // a, then b x3, a, b x3, then c: 9 calls.
    CHECK(g_calls == 9);
    const void* A = g_stream + a + 3; const void* B = g_stream + b + 3; const void* C = g_stream + c + 3;
    const void* want[9] = { A, B, B, B, A, B, B, B, C };
    unsigned long wantCount[9] = { 2, 1, 1, 1, 2, 1, 1, 1, 5 };
    for (int i = 0; i < 9; ++i) CHECK(g_payload[i] == want[i] && g_counts[i] == wantCount[i]);
    CHECK(LoopCount(program) == 0);

    // ApplyConstraints over a one-record program (DISTANCE).
    g_w = 0; Rec(2, 1, 1); g_length = g_w * 4;
    static const float script[] = { 5.0f, 3.0f, 1.0f, 0.5f };
    memcpy(g_script, script, sizeof(script)); g_scriptLen = 4;
    float junk = 9.0f; memcpy(sim + 0x30, &junk, 4);
    g_calls = 0; g_pass = 0;
    FixApply(program, 0, sim, 10, 1.0f);
    CHECK(g_calls == 3);                                      // 5, 3, then 1 <= 1 stops
    for (int i = 0; i < 3; ++i) CHECK(g_resetSeen[i] == 0.0f);   // reset before each pass
    g_calls = 0; g_pass = 0;
    FixApply(program, 0, sim, 2, 0.1f);
    CHECK(g_calls == 2);                                      // never converges: capped
    g_calls = 0; g_pass = 0;
    FixApply(program, 0, sim, 0, 0.1f);
    CHECK(g_calls == 0);

    if (g_fail) { printf("EXECUTE_PROGRAM_FAIL %d\n", g_fail); return 1; }
    printf("EXECUTE_PROGRAM_OK\n");
    return 0;
}
