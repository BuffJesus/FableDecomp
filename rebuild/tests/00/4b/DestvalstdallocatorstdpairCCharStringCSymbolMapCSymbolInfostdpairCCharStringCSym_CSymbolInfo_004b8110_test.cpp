#include <stdio.h>

static int g_seq;
static int g_order[8];

struct StrRefA {   // -> 0x99b510
    void* p;
    ~StrRefA() { g_order[g_seq++] = 0x100 + (int)(long)p; }
};

struct StrRefB {   // -> 0x99eae0
    void* p;
    ~StrRefB() { g_order[g_seq++] = 0x200 + (int)(long)p; }
};

struct SymPair_Dest_val {
    char     pad0[0x0c];
    StrRefB  m0c;        // +0x0c
    StrRefA  m10;        // +0x10
    StrRefA  m14;        // +0x14
    ~SymPair_Dest_val();
};

SymPair_Dest_val::~SymPair_Dest_val() {}

int main() {
    SymPair_Dest_val* z = 0;
    if ((char*)&z->m0c - (char*)z != 0x0c) { printf("BADOFF 0c\n"); return 1; }
    if ((char*)&z->m10 - (char*)z != 0x10) { printf("BADOFF 10\n"); return 1; }
    if ((char*)&z->m14 - (char*)z != 0x14) { printf("BADOFF 14\n"); return 1; }
    if (sizeof(SymPair_Dest_val) != 0x18) { printf("BADSIZE %d\n",(int)sizeof(SymPair_Dest_val)); return 1; }

    char buf[sizeof(SymPair_Dest_val)];
    SymPair_Dest_val* p = (SymPair_Dest_val*)buf;
    p->m14.p = (void*)1;
    p->m10.p = (void*)2;
    p->m0c.p = (void*)0;
    g_seq = 0;
    p->~SymPair_Dest_val();

    if (g_seq != 3) { printf("BADCOUNT %d\n", g_seq); return 1; }
    if (g_order[0] != 0x101) { printf("BADO0 %x\n", g_order[0]); return 1; }
    if (g_order[1] != 0x102) { printf("BADO1 %x\n", g_order[1]); return 1; }
    if (g_order[2] != 0x200) { printf("BADO2 %x\n", g_order[2]); return 1; }

    printf("DESTVAL_ORDER_OK\n");
    return 0;
}