#include <cstdio>

struct CInner { char pad[0xd7]; char flag; };
struct CGameScriptInterface { int dummy; CInner* inner; };

char __fastcall CGameScriptInterface_IsSavingEnabled(const CGameScriptInterface* self)
{
    return self->inner->flag;
}

int main()
{
    CInner a; a.flag = 5;
    CInner b; b.flag = 0;
    CGameScriptInterface s;
    s.inner = &a;
    if (CGameScriptInterface_IsSavingEnabled(&s) != 5) { std::printf("FAIL nonzero\n"); return 1; }
    s.inner = &b;
    if (CGameScriptInterface_IsSavingEnabled(&s) != 0) { std::printf("FAIL zero\n"); return 1; }
    std::printf("CGameScriptInterface_0088f950_TEST PASS\n");
    return 0;
}