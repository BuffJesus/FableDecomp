#include <stdio.h>

struct ISurface {
    virtual void Slot0(int a) = 0;
};

struct CEngineLandscapeMap {
    char pad0[4];
    ISurface* surface;
    char pad1[0x5d - 0x08];
    unsigned char flag;
};

static int g_released = 0;
static int g_arg = 0;

struct FakeSurface : public ISurface {
    void Slot0(int a) { g_released++; g_arg = a; }
};

void __fastcall ReleaseDefaultPoolSurfaces(CEngineLandscapeMap* self)
{
    if (self->flag) {
        ISurface* s = self->surface;
        if (s) {
            s->Slot0(1);
        }
        self->surface = 0;
    }
}

int main()
{
    FakeSurface fs;

    // Case 1: flag set, surface non-null -> release called with 1, surface nulled
    CEngineLandscapeMap a;
    a.flag = 1; a.surface = &fs;
    g_released = 0; g_arg = 0;
    ReleaseDefaultPoolSurfaces(&a);
    if (g_released != 1 || g_arg != 1 || a.surface != 0) { printf("FAIL1\n"); return 1; }

    // Case 2: flag set, surface null -> nothing released, still null
    CEngineLandscapeMap b;
    b.flag = 1; b.surface = 0;
    g_released = 0;
    ReleaseDefaultPoolSurfaces(&b);
    if (g_released != 0 || b.surface != 0) { printf("FAIL2\n"); return 1; }

    // Case 3: flag clear -> surface untouched, no release
    CEngineLandscapeMap c;
    c.flag = 0; c.surface = &fs;
    g_released = 0;
    ReleaseDefaultPoolSurfaces(&c);
    if (g_released != 0 || c.surface != &fs) { printf("FAIL3\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}