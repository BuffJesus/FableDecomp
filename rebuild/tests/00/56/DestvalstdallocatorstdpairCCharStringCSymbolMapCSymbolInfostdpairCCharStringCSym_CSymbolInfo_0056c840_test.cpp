#include <cstdio>

// Behaviour test: model the three sub-object destructors with observable
// side effects and confirm _Dest_val runs them in the exact retail order
// (member@0x18, member@0x14, member@0x10).

static int g_log[8];
static int g_n = 0;

struct CCharString {
    char* m_buf;
    ~CCharString();
};
struct CInfoHead {
    int m_a;
    ~CInfoHead();
};

struct SymbolPair {
    int         m_pad[4];   // +0x00..0x0F
    CInfoHead   m_head;     // +0x10
    CCharString m_s1;       // +0x14
    CCharString m_s2;       // +0x18
};

static SymbolPair* g_pair;

static int tag_of(const void* p) {
    CCharString* ps2   = &g_pair->m_s2;
    CCharString* ps1   = &g_pair->m_s1;
    CInfoHead*   phead = &g_pair->m_head;
    if (p == (const void*)ps2)   return 0x18;
    if (p == (const void*)ps1)   return 0x14;
    if (p == (const void*)phead) return 0x10;
    return -1;
}

CCharString::~CCharString() {
    g_log[g_n++] = tag_of(this);
}
CInfoHead::~CInfoHead() {
    g_log[g_n++] = tag_of(this);
}

void __fastcall Dest_val_CSymbolInfo___(SymbolPair* self, int edx);

int main() {
    SymbolPair obj;
    SymbolPair* self = &obj;
    g_pair = self;

    // verify reconstructed member offsets
    char* base    = (char*)self;
    char* pHead   = (char*)&self->m_head;
    char* pS1     = (char*)&self->m_s1;
    char* pS2     = (char*)&self->m_s2;
    unsigned oHead = (unsigned)(pHead - base);
    unsigned oS1   = (unsigned)(pS1 - base);
    unsigned oS2   = (unsigned)(pS2 - base);
    if (oHead != 0x10 || oS1 != 0x14 || oS2 != 0x18) {
        printf("BAD_OFFSETS %x %x %x\n", oHead, oS1, oS2);
        return 1;
    }

    g_n = 0;
    Dest_val_CSymbolInfo___(self, 0);

    if (g_n == 3 && g_log[0] == 0x18 && g_log[1] == 0x14 && g_log[2] == 0x10) {
        printf("DEST_VAL_OK\n");
        return 0;
    }
    printf("BAD_ORDER n=%d %x %x %x\n", g_n, g_log[0], g_log[1], g_log[2]);
    return 1;
}