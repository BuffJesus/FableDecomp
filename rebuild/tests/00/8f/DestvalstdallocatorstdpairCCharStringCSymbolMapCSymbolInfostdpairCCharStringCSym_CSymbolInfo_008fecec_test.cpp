#include <stdio.h>

struct CCharString {
    void* p;
    ~CCharString();
};

struct CSymbolInfo___ {
    CCharString first;
    int         pad0;
    int         pad1;
    int         c0;
    int         c1;
    CCharString s1;
    int         gap;
    CCharString s2;
    void Dest_val();
    void PairFirstDtor();
};

// order log to prove destruction sequence: s2, s1, first(base)
static int g_seq[8];
static int g_n = 0;

CCharString::~CCharString() {
    g_seq[g_n++] = (int)(long)this->p;
}

void CSymbolInfo___::PairFirstDtor() {
    g_seq[g_n++] = 999; // base/first dtor marker
}

int main() {
    CSymbolInfo___ pr;
    pr.s1.p = (void*)11;
    pr.s2.p = (void*)22;
    pr.first.p = (void*)1;

    g_n = 0;
    pr.Dest_val();

    // expected: s2 (22), s1 (11), then base (999)
    if (g_n == 3 && g_seq[0] == 22 && g_seq[1] == 11 && g_seq[2] == 999) {
        printf("DESTVAL_ORDER_OK\n");
        return 0;
    }
    printf("FAIL n=%d %d %d %d\n", g_n, g_seq[0], g_seq[1], g_seq[2]);
    return 1;
}