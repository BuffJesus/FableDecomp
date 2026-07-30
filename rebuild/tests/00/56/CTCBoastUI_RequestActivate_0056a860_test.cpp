#include <stdio.h>

struct CTCBoastUI {
    char pad0[0x18];
    unsigned char f18;
    char pad19[0x0f];
    unsigned char f28;
    unsigned char f29;
    unsigned char f2a;
};

void __fastcall RequestActivate(CTCBoastUI *self)
{
    if (!self->f2a && !self->f18 && !self->f28)
        self->f2a = 1;
}

static void call_ra(CTCBoastUI *p)
{
    __asm { mov ecx, p }
    __asm { call RequestActivate }
}

int main()
{
    CTCBoastUI o;
    int ok = 1;

    /* Case 1: all zero -> f2a becomes 1 */
    o.f18 = 0; o.f28 = 0; o.f2a = 0;
    call_ra(&o);
    if (o.f2a != 1) ok = 0;

    /* Case 2: f18 set -> no activation */
    o.f18 = 1; o.f28 = 0; o.f2a = 0;
    call_ra(&o);
    if (o.f2a != 0) ok = 0;

    /* Case 3: f28 set -> no activation */
    o.f18 = 0; o.f28 = 5; o.f2a = 0;
    call_ra(&o);
    if (o.f2a != 0) ok = 0;

    /* Case 4: f2a already set -> stays set, unchanged path */
    o.f18 = 0; o.f28 = 0; o.f2a = 1;
    call_ra(&o);
    if (o.f2a != 1) ok = 0;

    if (ok) { printf("OK_0x0056a860\n"); return 0; }
    printf("BAD_0x0056a860\n"); return 1;
}