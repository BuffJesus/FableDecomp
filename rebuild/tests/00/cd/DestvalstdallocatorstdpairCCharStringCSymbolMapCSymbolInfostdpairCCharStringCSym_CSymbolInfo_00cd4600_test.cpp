#include <stdio.h>

static int g_order[8];
static int g_count = 0;

struct CCharString {
    char* p;
};

void __fastcall CCharStringDtor(CCharString* self)
{
    if (self->p) { g_order[g_count++] = *self->p; }
    self->p = 0;
}

struct PairCharStringSymbolInfo {
    CCharString m0;
    CCharString m1;
    CCharString m2;
};

struct AllocDestVal_pair_CCharString_CSymbolInfo {
    void _Dest_val();
};

void AllocDestVal_pair_CCharString_CSymbolInfo::_Dest_val()
{
    PairCharStringSymbolInfo* self = reinterpret_cast<PairCharStringSymbolInfo*>(this);
    CCharStringDtor(&self->m2);
    CCharStringDtor(&self->m1);
    CCharStringDtor(&self->m0);
}

int main()
{
    char a = 0, b = 1, c = 2;
    PairCharStringSymbolInfo pr;
    pr.m0.p = &a;
    pr.m1.p = &b;
    pr.m2.p = &c;

    reinterpret_cast<AllocDestVal_pair_CCharString_CSymbolInfo*>(&pr)->_Dest_val();

    bool ok = (g_count == 3) &&
              (g_order[0] == 2) &&   // m2 first
              (g_order[1] == 1) &&   // then m1
              (g_order[2] == 0) &&   // then m0
              (pr.m0.p == 0) && (pr.m1.p == 0) && (pr.m2.p == 0);

    if (ok) printf("DESTVAL_OK order=%d,%d,%d\n", g_order[0], g_order[1], g_order[2]);
    else    printf("DESTVAL_FAIL count=%d\n", g_count);
    return ok ? 0 : 1;
}