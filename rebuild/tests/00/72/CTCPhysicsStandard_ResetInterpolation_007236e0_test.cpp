#include <cstdio>

struct Xform6 { unsigned int m[6]; };
struct CTCPhysicsStandard {
    char pad0[0x50];
    Xform6 cur;         // +0x50
    Xform6 interp;      // +0x68
    unsigned int flag;  // +0x80
};

int g_helper_called = 0;
int g_tail_called = 0;

void __fastcall CTCPhysicsStandard_helper(CTCPhysicsStandard* self) { g_helper_called = 1; (void)self; }
unsigned int __cdecl CTCPhysicsStandard_getFlag(void) { return 0xABCD1234u; }
void __fastcall CTCPhysicsStandard_tail(CTCPhysicsStandard* self) { g_tail_called = 1; (void)self; }

void __fastcall CTCPhysicsStandard_ResetInterpolation(CTCPhysicsStandard* self)
{
    self->interp = self->cur;
    CTCPhysicsStandard_helper(self);
    self->flag = CTCPhysicsStandard_getFlag();
    CTCPhysicsStandard_tail(self);
}

int main()
{
    CTCPhysicsStandard obj;
    for (int i = 0; i < 6; ++i) { obj.cur.m[i] = 0x1000u + i; obj.interp.m[i] = 0; }
    obj.flag = 0;
    CTCPhysicsStandard_ResetInterpolation(&obj);
    int ok = 1;
    for (int i = 0; i < 6; ++i) if (obj.interp.m[i] != 0x1000u + i) ok = 0;
    if (obj.flag != 0xABCD1234u) ok = 0;
    if (!g_helper_called || !g_tail_called) ok = 0;
    if (ok) { std::printf("CTCPhysicsStandard_007236e0_TEST PASS\n"); return 0; }
    std::printf("CTCPhysicsStandard_007236e0_TEST FAIL\n");
    return 1;
}