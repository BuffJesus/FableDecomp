#include <cstdio>

struct Self;
struct VTbl {
    void* s0; void* s1; void* s2; void* s3;
    Self* (__fastcall* clone)(Self* self);
};
struct Self {
    VTbl* vt; int f4; int f8; int fc;
};

static Self g_new;
static Self* __fastcall clone_impl(Self* self) {
    g_new.vt = self->vt;
    g_new.f4 = 999;
    g_new.f8 = 0;
    g_new.fc = 0;
    return &g_new;
}

Self* __fastcall Clone_CombatHandedness(Self* self)
{
    Self* n = self->vt->clone(self);
    n->f8 = self->f8;
    n->fc = self->fc;
    return n;
}

int main() {
    VTbl vt = {0,0,0,0, clone_impl};
    Self s;
    s.vt = &vt; s.f4 = 1; s.f8 = 0x1234; s.fc = 0x5678;
    Self* r = Clone_CombatHandedness(&s);
    bool ok = (r == &g_new) && (r->f8 == 0x1234) && (r->fc == 0x5678) && (r->f4 == 999);
    // second case: different values
    s.f8 = -7; s.fc = 42;
    Self* r2 = Clone_CombatHandedness(&s);
    ok = ok && (r2->f8 == -7) && (r2->fc == 42);
    printf(ok ? "OK_005db260\n" : "FAIL\n");
    return ok ? 0 : 1;
}