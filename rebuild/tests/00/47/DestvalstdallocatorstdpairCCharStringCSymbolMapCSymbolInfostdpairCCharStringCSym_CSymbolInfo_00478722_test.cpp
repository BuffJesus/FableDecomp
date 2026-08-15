#include <cstdio>

struct CCharString { void* p; };
struct InnerVal { void* a; void* b; };
struct CSymbolInfo { InnerVal inner; };
struct SymPair {
    CCharString first;
    char pad[0x40 - sizeof(CCharString)];
    CSymbolInfo second;
};

static int order = 0;
static int inner_ok = 0, sec_ok = 0, first_ok = 0;

void __fastcall destroy_inner_val(void* a, void* b) {
    if (a == (void*)0x11 && b == (void*)0x22 && order == 0) { inner_ok = 1; order = 1; }
}
void __fastcall symbolinfo_dtor(CSymbolInfo* s) {
    if (order == 1) { sec_ok = 1; order = 2; }
}
void __fastcall ccharstring_dtor(CCharString* s) {
    if (order == 2) { first_ok = 1; order = 3; }
}

void __fastcall _Dest_val_CSymbolInfo___(SymPair* p) {
    CSymbolInfo* sec = &p->second;
    destroy_inner_val(sec->inner.a, sec->inner.b);
    symbolinfo_dtor(sec);
    ccharstring_dtor(&p->first);
}

int main() {
    SymPair pr;
    pr.first.p = (void*)0x99;
    pr.second.inner.a = (void*)0x11;
    pr.second.inner.b = (void*)0x22;
    _Dest_val_CSymbolInfo___(&pr);
    if (inner_ok && sec_ok && first_ok && order == 3)
        printf("DESTVAL_OK\n");
    else
        printf("FAIL order=%d %d %d %d\n", order, inner_ok, sec_ok, first_ok);
    return 0;
}