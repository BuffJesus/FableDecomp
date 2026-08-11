#include <stdio.h>

// Mirror of the source types (independent test).
struct CCharString {
    char* p;
    ~CCharString();
};
struct CSymbolInfo {
    int slot;
    ~CSymbolInfo();
};
struct SymPair {
    char        head[0x0c];
    CSymbolInfo second;
    CCharString a;
    CCharString b;
    void _Dest_val_CSymbolInfo___();
};

// Order-tracking definitions of the "out-of-line" destructors.
static int g_seq = 0;
static int g_order[3];
static int g_n = 0;

CCharString::~CCharString() { g_order[g_n++] = 1; }
CSymbolInfo::~CSymbolInfo() { g_order[g_n++] = 2; }

void SymPair::_Dest_val_CSymbolInfo___() {
    this->b.~CCharString();
    this->a.~CCharString();
    this->second.~CSymbolInfo();
}

int main() {
    SymPair sp;
    g_n = 0;
    sp._Dest_val_CSymbolInfo___();
    // Expect exactly: CCharString, CCharString, CSymbolInfo (1,1,2).
    if (g_n == 3 && g_order[0]==1 && g_order[1]==1 && g_order[2]==2) {
        printf("DEST_VAL_ORDER_OK\n");
        return 0;
    }
    printf("DEST_VAL_ORDER_BAD n=%d %d %d %d\n", g_n, g_order[0], g_order[1], g_order[2]);
    return 1;
}