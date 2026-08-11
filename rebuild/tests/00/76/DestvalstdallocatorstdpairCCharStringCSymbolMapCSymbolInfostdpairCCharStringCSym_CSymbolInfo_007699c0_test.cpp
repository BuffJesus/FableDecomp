#include <cstdio>

// ---- Local stand-ins for the leaf destructors (record destruction order) ----
static int g_seq[8];
static int g_n = 0;

class CCharString {
public:
    ~CCharString();
    char* m_p;
};
class CBaseIntelligentPointer {
public:
    ~CBaseIntelligentPointer();
    void* m_p;
};

// tag which subobject via its address relative to the pair base
static void* g_base = 0;
CCharString::~CCharString()             { g_seq[g_n++] = (int)((char*)this - (char*)g_base); }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { g_seq[g_n++] = (int)((char*)this - (char*)g_base); }

struct CSymbolInfo {
    CCharString name;
    int a;
    int b;
    CBaseIntelligentPointer ptr;
};
struct SymPair {
    CCharString first;
    CSymbolInfo second;
};

void __fastcall Dest_val_CSymbolInfo___(SymPair* _Ptr)
{
    _Ptr->~SymPair();
}

int main()
{
    // raw buffer so we control construction / avoid double-destroy
    static char buf[64];
    SymPair* p = (SymPair*)buf;
    g_base = buf;
    g_n = 0;

    // placement-init the vptr for the intelligent pointer member so the
    // member dtor path is well-formed (no constructor side effects needed
    // for the sequence test)
    // NOTE: we call Dest_val on a POD-ish layout; the leaf dtors just log.
    Dest_val_CSymbolInfo___(p);

    // Expected order: second.ptr (+0x10), second.name (+4), first (+0)
    bool ok = (g_n == 3) &&
              (g_seq[0] == 0x10) &&
              (g_seq[1] == 0x04) &&
              (g_seq[2] == 0x00);

    printf("seq n=%d [%d,%d,%d]\n", g_n, g_seq[0], g_seq[1], g_seq[2]);
    if (ok) { printf("DESTVAL_ORDER_OK\n"); return 0; }
    printf("DESTVAL_ORDER_FAIL\n");
    return 1;
}