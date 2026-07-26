#include <cstdio>

struct Decal { int a; int b; void** obj; int d; char active; char pad[7]; };
struct CParticleSystem { char pad0[0xc]; Decal* begin; Decal* end; char pad1[0x34]; long base; };
typedef long (__fastcall *Fn)(void* self);
long __fastcall CParticleSystem_GetNoActiveDecals(CParticleSystem* self)
{
    long acc = 0;
    Decal* it = self->begin;
    if (it != self->end) {
        void** p = (void**)((char*)it + 8);
        do {
            if (((char*)p)[8]) {
                void** obj = (void**)*p;
                acc += (*(Fn*)((char*)*obj + 0x68))(obj);
            }
            ++it;
            p = (void**)((char*)p + 0x18);
        } while (it != self->end);
    }
    return self->base + acc;
}

static long __fastcall count5(void* self) { (void)self; return 5; }
static long __fastcall count3(void* self) { (void)self; return 3; }

int main()
{
    void* vtA[0x20]; void* vtB[0x20];
    for (int i = 0; i < 0x20; ++i) { vtA[i] = 0; vtB[i] = 0; }
    vtA[0x1a] = (void*)&count5;   /* +0x68 */
    vtB[0x1a] = (void*)&count3;

    void* objA[1]; objA[0] = (void*)vtA;
    void* objB[1]; objB[0] = (void*)vtB;

    Decal decals[3];
    for (int i = 0; i < 3; ++i) {
        decals[i].a = 0; decals[i].b = 0; decals[i].d = 0;
        decals[i].active = 0; decals[i].obj = 0;
    }
    decals[0].obj = (void**)objA; decals[0].active = 1;  /* +5 */
    decals[1].obj = (void**)objB; decals[1].active = 0;  /* skipped */
    decals[2].obj = (void**)objB; decals[2].active = 1;  /* +3 */

    CParticleSystem sys;
    sys.begin = &decals[0];
    sys.end   = &decals[3];
    sys.base  = 100;

    long r = CParticleSystem_GetNoActiveDecals(&sys);
    long expected = 100 + 5 + 3;

    /* empty-list path: begin==end returns base only */
    CParticleSystem empty;
    empty.begin = &decals[0];
    empty.end   = &decals[0];
    empty.base  = 42;
    long r2 = CParticleSystem_GetNoActiveDecals(&empty);

    if (r == expected && r2 == 42) {
        std::printf("CParticleSystem_00af3f30_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %ld exp %ld  r2 %ld\n", r, expected, r2);
    return 1;
}