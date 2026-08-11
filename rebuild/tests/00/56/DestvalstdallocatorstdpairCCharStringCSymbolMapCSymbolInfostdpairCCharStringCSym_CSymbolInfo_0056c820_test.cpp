#include <stdio.h>

// Independent behaviour test: verify the pair destructor destroys members in
// reverse declaration order (+0x18, +0x14, +0x10 == subB, subA, first).

static int g_order[3];
static int g_n = 0;

struct StrB {
    int id;
    ~StrB() { g_order[g_n++] = id; }
};

struct StrA {
    int id;
    ~StrA() { g_order[g_n++] = id; }
};

struct SymPair {
    char _pad[0x10];
    StrA first;   // +0x10, id=1
    StrB subA;    // +0x14, id=2
    StrB subB;    // +0x18, id=3
    SymPair() { first.id = 1; subA.id = 2; subB.id = 3; }
};

int main() {
    {
        SymPair p;
    } // dtor runs here
    // Expect reverse declaration order: subB(3), subA(2), first(1)
    if (g_n == 3 && g_order[0] == 3 && g_order[1] == 2 && g_order[2] == 1) {
        printf("DEST_ORDER_OK\n");
        return 0;
    }
    printf("DEST_ORDER_FAIL %d %d %d %d\n", g_n, g_order[0], g_order[1], g_order[2]);
    return 1;
}