#include <stdio.h>

struct A {
    char* buf;
    int   a;
    int   b;
    ~A();
};
struct B {
    int x;
    ~B();
};
struct DestPair {
    char pad0[0x0c];
    A    first;
    char pad1[0x08];
    A    second;
    char pad2[0x08];
    B    third;
};

void __fastcall Dest_val_CSymbolInfo___(DestPair* self);

static int g_a_dtor = 0;
static int g_b_dtor = 0;
static int g_order[4];
static int g_idx = 0;

A::~A() { g_a_dtor++; g_order[g_idx++] = 1; }
B::~B() { g_b_dtor++; g_order[g_idx++] = 2; }

int main()
{
    DestPair p;
    p.first.a = 0; p.second.a = 0; p.third.x = 0;
    Dest_val_CSymbolInfo___(&p);

    // Expect: 2 A-dtors, 1 B-dtor, in order first(A), second(A), third(B).
    if (g_a_dtor == 2 && g_b_dtor == 1 &&
        g_order[0] == 1 && g_order[1] == 1 && g_order[2] == 2) {
        printf("DESTVAL_OK\n");
        return 0;
    }
    printf("DESTVAL_FAIL a=%d b=%d\n", g_a_dtor, g_b_dtor);
    return 1;
}