#include <cstdio>

static int g_seq = 0;
static int g_order[3] = { 0, 0, 0 };

struct CCharString {
    char* buf;
    int   a;
    int   b;
    ~CCharString() { g_order[g_seq++] = 3; } // 0x99a2e0
};
struct SubB {
    char* p;
    int   q;
    ~SubB() { g_order[g_seq++] = 2; } // 0x99eae0
};
struct SubA {
    char* p;
    int   q;
    int   r;
    ~SubA() { g_order[g_seq++] = 1; } // 0xa01c10
};
struct Pair {
    CCharString first;
    SubB        b;
    SubA        a;
};

void __fastcall _Dest_val_CSymbolInfo___(Pair* _Ptr, int)
{
    _Ptr->a.~SubA();
    _Ptr->b.~SubB();
    _Ptr->first.~CCharString();
}

int main()
{
    // raw storage so we don't double-destroy
    char storage[sizeof(Pair)];
    Pair* p = (Pair*)storage;
    p->first.buf = 0; p->first.a = 0; p->first.b = 0;
    p->b.p = 0; p->b.q = 0;
    p->a.p = 0; p->a.q = 0; p->a.r = 0;

    _Dest_val_CSymbolInfo___(p, 0);

    // Expect reverse-member order: SubA(1), SubB(2), CCharString(3)
    if (g_seq == 3 && g_order[0] == 1 && g_order[1] == 2 && g_order[2] == 3) {
        printf("DESTVAL_ORDER_OK\n");
        return 0;
    }
    printf("DESTVAL_ORDER_BAD seq=%d %d %d %d\n", g_seq, g_order[0], g_order[1], g_order[2]);
    return 1;
}