#include <cstdio>

struct CDecalGroup {
    int count;
    int GetCount() const { return count; }
};

struct Pair {
    CDecalGroup* group;
    int other;
};

struct CParticleEmitter {
    int a;
    int b;
    Pair* begin;
    Pair* end;
};

bool __fastcall CParticleEmitter_HasAllocatedDecals(const CParticleEmitter* self)
{
    Pair* it = self->begin;
    if (it != self->end) {
        do {
            if (it->group->GetCount() > 0)
                return true;
            ++it;
        } while (it != self->end);
    }
    return false;
}

int main()
{
    CDecalGroup g0; g0.count = 0;
    CDecalGroup g1; g1.count = 5;

    Pair empty[2]; empty[0].group = &g0; empty[1].group = &g0;
    CParticleEmitter e;
    e.begin = empty; e.end = empty + 2;
    if (CParticleEmitter_HasAllocatedDecals(&e)) { std::printf("FAIL: expected false\n"); return 1; }

    Pair some[2]; some[0].group = &g0; some[1].group = &g1;
    e.begin = some; e.end = some + 2;
    if (!CParticleEmitter_HasAllocatedDecals(&e)) { std::printf("FAIL: expected true\n"); return 1; }

    // empty range
    e.begin = some; e.end = some;
    if (CParticleEmitter_HasAllocatedDecals(&e)) { std::printf("FAIL: expected false empty\n"); return 1; }

    std::printf("CParticleEmitter_00aed220_TEST PASS\n");
    return 0;
}