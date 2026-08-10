#include <stdio.h>

struct Obj {
    char pad[0xC0];
    int cooldown;
};

extern "C" void __fastcall FrameUpdate(Obj* self);

static int g_base_called = 0;
extern "C" void __fastcall BaseFrameUpdate(Obj* self)
{
    g_base_called = 1;
}

int main()
{
    Obj a;
    a.cooldown = 3;
    g_base_called = 0;
    FrameUpdate(&a);
    if (a.cooldown != 2) { printf("FAIL dec\n"); return 1; }
    if (g_base_called != 0) { printf("FAIL base1\n"); return 1; }

    FrameUpdate(&a);
    if (a.cooldown != 1) { printf("FAIL dec2\n"); return 1; }

    a.cooldown = 0;
    g_base_called = 0;
    FrameUpdate(&a);
    if (g_base_called != 1) { printf("FAIL base0\n"); return 1; }

    printf("OK_008d5000\n");
    return 0;
}