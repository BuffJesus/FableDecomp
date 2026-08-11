#include <cstdio>

// Self-contained behaviour test for _Dest_val: verifies the pair is destroyed
// by running the two trailing CCharString member dtors (offsets +0x34 then
// +0x30) followed by the leading FirstPart dtor at offset 0.

static int g_seq = 0;
static int g_order[3];
static int g_n = 0;

struct CCharString {
    int tag;
    char* buf;              // observed member
    ~CCharString() { g_order[g_n++] = tag; }
};

struct FirstPart {
    char pad[0x30];
    int tag;
    ~FirstPart() { g_order[g_n++] = tag; }
};

struct PairVal {
    FirstPart first;
    CCharString s0;   // +0x30 region
    CCharString s1;   // +0x34 region
};

void __fastcall _Dest_val_CSymbolInfo___(PairVal* self)
{
    self->~PairVal();
}

int main()
{
    // Build a pair on raw storage so we control tags without double-destroy.
    char storage[sizeof(PairVal)];
    PairVal* p = reinterpret_cast<PairVal*>(storage);
    p->first.tag = 1;   // destroyed last
    p->s0.tag    = 2;   // destroyed 2nd  (+0x30)
    p->s1.tag    = 3;   // destroyed 1st  (+0x34)

    _Dest_val_CSymbolInfo___(p);

    // Expected reverse decl order: s1(3), s0(2), first(1).
    bool ok = (g_n == 3) &&
              (g_order[0] == 3) &&
              (g_order[1] == 2) &&
              (g_order[2] == 1);

    if (ok) {
        printf("DESTVAL_ORDER_OK\n");
        return 0;
    }
    printf("DESTVAL_FAIL n=%d [%d %d %d]\n", g_n, g_order[0], g_order[1], g_order[2]);
    return 1;
}