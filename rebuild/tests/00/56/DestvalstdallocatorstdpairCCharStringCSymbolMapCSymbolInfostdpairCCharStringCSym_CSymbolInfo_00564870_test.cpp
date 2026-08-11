#include <stdio.h>

static int g_order[8];
static int g_n = 0;

struct B {
    char* p;
    ~B();
};
struct A {
    int x;
    ~A();
};
B::~B() { g_order[g_n++] = 2; }   // records a 'B' destruction
A::~A() { g_order[g_n++] = 1; }   // records an 'A' destruction

struct Pair {
    B m0;
    int pad;
    B m8;
    A mc;
};

void __fastcall _Dest_val_CSymbolInfo___(Pair* self)
{
    self->mc.~A();
    self->m8.~B();
    self->m0.~B();
}

int main()
{
    Pair p;
    p.m0.p = 0; p.m8.p = 0; p.mc.x = 0; p.pad = 0;
    _Dest_val_CSymbolInfo___(&p);

    // Expected destruction order: A (mc), B (m8), B (m0) -> 1,2,2
    if (g_n == 3 && g_order[0] == 1 && g_order[1] == 2 && g_order[2] == 2) {
        printf("DESTVAL_ORDER_OK\n");
        return 0;
    }
    printf("DESTVAL_FAIL n=%d %d %d %d\n", g_n, g_order[0], g_order[1], g_order[2]);
    return 1;
}