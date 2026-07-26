#include <cstdio>

static int g_ret = 0;
struct CDecalThing {
    int __fastcall GetDecalCount();
};
int __fastcall CDecalThing::GetDecalCount() { return g_ret; }

struct Pair {
    CDecalThing* obj;
    void* other;
};

struct CParticleEmitter {
    char pad[8];
    Pair* begin;
    Pair* end;
};

bool __fastcall CParticleEmitter_HasAllocatedDecals(CParticleEmitter* self)
{
    Pair* it = self->begin;
    if (it != self->end) {
        do {
            if (it->obj->GetDecalCount() > 0)
                return true;
            it++;
        } while (it != self->end);
    }
    return false;
}

int main() {
    CDecalThing d;
    Pair arr[3];
    arr[0].obj = &d; arr[1].obj = &d; arr[2].obj = &d;
    CParticleEmitter e;
    e.begin = arr; e.end = arr + 3;

    g_ret = 0;
    if (CParticleEmitter_HasAllocatedDecals(&e)) { std::printf("FAIL empty-decal\n"); return 1; }

    g_ret = 5;
    if (!CParticleEmitter_HasAllocatedDecals(&e)) { std::printf("FAIL has-decal\n"); return 1; }

    CParticleEmitter empty;
    empty.begin = arr; empty.end = arr;
    if (CParticleEmitter_HasAllocatedDecals(&empty)) { std::printf("FAIL empty-range\n"); return 1; }

    std::printf("CParticleEmitter_00aed1b0_TEST PASS\n");
    return 0;
}