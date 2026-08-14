#include <stdio.h>

struct D { long dummy[7]; long frame; };
struct C { char pad[0x2c]; D* pD; };
struct B { char pad0[8]; C* pC; };
struct A { char pad0[0x88]; B* pB; };

A* g_singleton = 0;

long model()
{
    B* b = g_singleton->pB;
    if (b) {
        C* c = b->pC;
        if (c) {
            D* d = c->pD;
            return d->frame;
        }
    }
    return 0;
}

int main()
{
    A a; B b; C c; D d;
    d.frame = 4242;
    c.pD = &d;
    b.pC = &c;
    a.pB = &b;
    g_singleton = &a;

    int ok = 1;
    if (model() != 4242) ok = 0;      // full chain

    a.pB = 0;                         // b null
    if (model() != 0) ok = 0;

    a.pB = &b; b.pC = 0;              // c null
    if (model() != 0) ok = 0;

    if (ok) printf("BEHAVIOUR_OK\n");
    else printf("BEHAVIOUR_FAIL\n");
    return 0;
}