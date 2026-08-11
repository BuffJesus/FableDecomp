#include <stdio.h>

struct Sub64;
extern void __fastcall sub_4bfa50(Sub64* p, int edx, float scale);

struct CThingPhysical {
    char pad[0x64];
    Sub64* f64;      // +0x64
    char pad2[0x9c - 0x64 - 4];
    float f9c;       // +0x9c
};

void __fastcall CThingPhysical_SetScale(CThingPhysical* self, int edx, float scale);

static int g_called = 0;
static Sub64* g_arg = 0;
static float g_scale = 0.0f;
void __fastcall sub_4bfa50(Sub64* p, int edx, float scale) { g_called++; g_arg = p; g_scale = scale; }

int main()
{
    CThingPhysical o;
    for (int i = 0; i < (int)sizeof(o); ++i) ((char*)&o)[i] = 0;

    // case 1: f64 null -> no tail call, scale stored
    o.f64 = 0;
    CThingPhysical_SetScale(&o, 0, 3.5f);
    bool ok1 = (o.f9c == 3.5f) && (g_called == 0);

    // case 2: f64 non-null -> tail call with f64 and scale
    Sub64* fake = (Sub64*)0x12345678;
    o.f64 = fake;
    CThingPhysical_SetScale(&o, 0, 7.25f);
    bool ok2 = (o.f9c == 7.25f) && (g_called == 1) && (g_arg == fake) && (g_scale == 7.25f);

    if (ok1 && ok2)
        printf("006a4b80_TEST PASS\n");
    else
        printf("FAIL ok1=%d ok2=%d f9c=%f called=%d scale=%f\n", ok1, ok2, o.f9c, g_called, g_scale);
    return 0;
}