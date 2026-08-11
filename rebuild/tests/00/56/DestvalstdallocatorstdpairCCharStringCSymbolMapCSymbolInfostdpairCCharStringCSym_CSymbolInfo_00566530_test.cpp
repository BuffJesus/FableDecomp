#include <cstdio>

struct CWideString {
    unsigned short* m_pData;
    ~CWideString();
};
struct CSymOther {
    int a;
    ~CSymOther();
};
struct SymPair {
    CWideString first;
    int         _pad;
    CWideString sec_name;
    CSymOther   sec_x;
};

void __fastcall _Dest_val_CSymbolInfo___SymPair(SymPair* p);

static int g_seq = 0;
static int g_order[3];

CWideString::~CWideString() {
    g_order[g_seq] = 5;   // CWideString marker
    g_seq++;
}
CSymOther::~CSymOther() {
    g_order[g_seq] = 9;   // Other marker
    g_seq++;
}

int main() {
    SymPair p;
    p.first.m_pData = 0;
    p.sec_name.m_pData = 0;
    p.sec_x.a = 0;

    _Dest_val_CSymbolInfo___SymPair(&p);

    // Expected destruction order: sec_x (9), sec_name (5), first (5)
    bool ok = (g_seq == 3) && (g_order[0] == 9) && (g_order[1] == 5) && (g_order[2] == 5);
    if (ok) {
        printf("DESTVAL_PAIR_OK\n");
        return 0;
    }
    printf("DESTVAL_PAIR_FAIL seq=%d o=%d,%d,%d\n", g_seq, g_order[0], g_order[1], g_order[2]);
    return 1;
}