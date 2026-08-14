#include <cstdio>

struct L3 { char pad18[0x18]; long value; };
struct L2 { char pad2c[0x2c]; L3* p2c; };
struct L1 { char pad08[0x08]; L2* p08; };
struct Root { char pad88[0x88]; L1* p88; };

Root* g_root;

long model()
{
    L1* a = g_root->p88;
    if (a) {
        L2* b = a->p08;
        if (b) {
            return b->p2c->value;
        }
    }
    return 0;
}

int main()
{
    // Case 1: full chain -> value
    L3 l3; l3.value = 0x1234;
    L2 l2; l2.p2c = &l3;
    L1 l1; l1.p08 = &l2;
    Root r; r.p88 = &l1;
    g_root = &r;
    if (model() != 0x1234) { printf("FAIL a\n"); return 1; }

    // Case 2: p88 null -> 0
    r.p88 = 0;
    if (model() != 0) { printf("FAIL b\n"); return 1; }

    // Case 3: p88 set but p08 null -> 0
    r.p88 = &l1; l1.p08 = 0;
    if (model() != 0) { printf("FAIL c\n"); return 1; }

    printf("BEHAVIOR_OK\n");
    return 0;
}