#include <cstdio>

struct Base;
struct Vtbl {
    int (__fastcall *pad[28])(Base*);
    int (__fastcall *GetPointType)(Base*);
};
struct Base { Vtbl* vtbl; };

bool __fastcall GetPointType_impl(Base* self, void*, Base** out)
{
    if (self->vtbl->GetPointType(self) == 7) { *out = self; return true; }
    return false;
}

static int g_type = 0;
int __fastcall gpt(Base*) { return g_type; }

int main()
{
    Vtbl v;
    v.GetPointType = gpt;
    Base b; b.vtbl = &v;

    Base* out = 0;

    g_type = 7;
    out = 0;
    bool r1 = GetPointType_impl(&b, 0, &out);
    if (!(r1 == true && out == &b)) { printf("BAD1\n"); return 1; }

    g_type = 3;
    out = 0;
    bool r2 = GetPointType_impl(&b, 0, &out);
    if (!(r2 == false && out == 0)) { printf("BAD2\n"); return 1; }

    g_type = 8;
    out = (Base*)0xdead;
    bool r3 = GetPointType_impl(&b, 0, &out);
    if (!(r3 == false)) { printf("BAD3\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}