#include <cstdio>

struct DecalObjVtbl {
    long (__fastcall* fns[64])(void*);
};

struct DecalObj {
    DecalObjVtbl* vtbl;
};

struct CParticleSystem {
    char pad0[0xC];
    char* begin;
    char* end;
    char pad14[0x4C - 0x14];
    long base;
};

long __fastcall CParticleSystem_GetNoActiveDecals(CParticleSystem* self)
{
    long total = 0;
    char* p = (char*)self->begin;
    char* endp = (char*)self->end;
    if (p != endp) {
        char* q = p + 8;
        do {
            if (*(char*)(q + 8)) {
                DecalObj* o = *(DecalObj**)q;
                total += o->vtbl->fns[0x6c / 4](o);
            }
            endp = (char*)self->end;
            p += 0x18;
            q += 0x18;
        } while (p != endp);
    }
    return self->base + total;
}

static long __fastcall countA(void* o) { return 5; }

int main() {
    DecalObjVtbl vt;
    for (int i = 0; i < 64; ++i) vt.fns[i] = 0;
    vt.fns[0x6c/4] = countA;
    DecalObj obj; obj.vtbl = &vt;

    // element is 0x18 bytes: obj ptr at +8, flag at +0x10
    char buf[0x18 * 3];
    for (int i = 0; i < 0x18 * 3; ++i) buf[i] = 0;
    for (int i = 0; i < 3; ++i) {
        *(DecalObj**)(buf + i*0x18 + 8) = &obj;
        buf[i*0x18 + 0x10] = 0;
    }
    buf[0*0x18 + 0x10] = 1;   // contributes 5
    buf[2*0x18 + 0x10] = 1;   // contributes 5

    CParticleSystem sys;
    sys.begin = buf;
    sys.end = buf + 0x18*3;
    sys.base = 100;

    long r = CParticleSystem_GetNoActiveDecals(&sys);
    long expected = 100 + 5 + 5;
    if (r != expected) {
        std::printf("FAIL got %ld expected %ld\n", r, expected);
        return 1;
    }

    sys.end = buf;  // empty range
    if (CParticleSystem_GetNoActiveDecals(&sys) != 100) {
        std::printf("FAIL empty case\n");
        return 1;
    }

    std::printf("CParticleSystem_00af3f70_TEST PASS\n");
    return 0;
}