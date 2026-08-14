#include <stdio.h>

struct GlobalObj { int tag; };
struct MidObj { int tag; };
struct Self { int shown; };

static int g_calls = 0;
static GlobalObj g_obj = { 111 };
GlobalObj* g_global = &g_obj;

MidObj g_mid = { 222 };

MidObj* __fastcall Global_Get(GlobalObj* self, int edx, int idx)
{
    if (self == &g_obj && idx == 0xa) g_calls |= 1;
    return &g_mid;
}
void __fastcall Mid_Method(MidObj* self)
{
    if (self == &g_mid) g_calls |= 2;
}
void __fastcall Self_Show(Self* self)
{
    if (self) { self->shown = 1; g_calls |= 4; }
}

void __fastcall OpenInventory(Self* self)
{
    Mid_Method(Global_Get(g_global, 0, 0xa));
    Self_Show(self);
}

int main(void)
{
    Self s = { 0 };
    OpenInventory(&s);
    if (g_calls == 7 && s.shown == 1)
        printf("PARITY_OK\n");
    else
        printf("FAIL %d %d\n", g_calls, s.shown);
    return 0;
}