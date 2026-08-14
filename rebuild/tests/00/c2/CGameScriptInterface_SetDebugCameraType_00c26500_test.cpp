#include <cstdio>

struct Divisor;
struct DivisorVtbl {
    unsigned int (__fastcall *slot0)(Divisor* self);
    unsigned int (__fastcall *slot1)(Divisor* self);
    unsigned int (__fastcall *slot2)(Divisor* self);
};
struct Divisor { DivisorVtbl* vtbl; };
struct Holder { unsigned int slot0; Divisor* obj; };
struct Self {
    char pad0[0x14];
    unsigned int lo;
    char pad1[0x1c - 0x18];
    unsigned int hi;
    Holder* holder;
};

unsigned int __fastcall SetDebugCameraType(Self* self)
{
    unsigned int span = self->hi - self->lo;
    Divisor* d = self->holder->obj;
    unsigned int divisor = d->vtbl->slot2(d);
    return span / divisor;
}

static unsigned int g_div;
static unsigned int __fastcall retDiv(Divisor*) { return g_div; }

int main()
{
    DivisorVtbl vt; vt.slot0 = 0; vt.slot1 = 0; vt.slot2 = retDiv;
    Divisor d; d.vtbl = &vt;
    Holder h; h.slot0 = 0; h.obj = &d;
    Self s;
    s.lo = 100; s.hi = 1000; s.holder = &h;

    g_div = 30;   // (1000-100)/30 = 30
    unsigned int r1 = SetDebugCameraType(&s);
    g_div = 7;    // 900/7 = 128
    unsigned int r2 = SetDebugCameraType(&s);

    if (r1 == 30 && r2 == 128) {
        printf("BEHAVIOUR_OK\n");
    } else {
        printf("BEHAVIOUR_FAIL r1=%u r2=%u\n", r1, r2);
    }
    return 0;
}