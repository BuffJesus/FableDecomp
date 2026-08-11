#include <stdio.h>

struct CCharString;
struct SymbolPair;

extern "C" void __fastcall CCharString_Dtor(CCharString *self);
extern "C" void __fastcall SymbolPair_Dtor(SymbolPair *self);

struct CCharString {};

struct SymbolPair
{
    char        pad0[0x14];
    CCharString strA;
    char        pad1[0x30 - 0x14 - 1];
    CCharString strB;
};

void __fastcall _Dest_val_SymbolPair(SymbolPair *self, SymbolPair *unused)
{
    CCharString_Dtor(&self->strB);
    CCharString_Dtor(&self->strA);
    SymbolPair_Dtor(self);
}

// --- test harness: observe call order & targets ---
static int g_log[8];
static int g_n = 0;
static SymbolPair *g_base;

extern "C" void __fastcall CCharString_Dtor(CCharString *self)
{
    // record byte offset of the subobject relative to base
    g_log[g_n++] = (int)((char*)self - (char*)g_base);
}
extern "C" void __fastcall SymbolPair_Dtor(SymbolPair *self)
{
    g_log[g_n++] = (self == g_base) ? -1 : -999;
}

int main()
{
    SymbolPair p;
    g_base = &p;
    g_n = 0;
    _Dest_val_SymbolPair(&p, &p);

    // Expected order: strB (+0x30), strA (+0x14), pair dtor (self => -1)
    if (g_n == 3 && g_log[0] == 0x30 && g_log[1] == 0x14 && g_log[2] == -1)
        printf("DESTVAL_ORDER_OK\n");
    else
        printf("FAIL n=%d o0=%d o1=%d o2=%d\n", g_n, g_log[0], g_log[1], g_log[2]);
    return (g_n == 3 && g_log[0] == 0x30 && g_log[1] == 0x14 && g_log[2] == -1) ? 0 : 1;
}