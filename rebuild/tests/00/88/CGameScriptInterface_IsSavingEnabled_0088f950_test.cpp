#include "engine/CGameScriptInterface.h"
#include <cstdio>

struct CWorld { char pad[0xd7]; char flag; };

char __fastcall CGameScriptInterface_IsSavingEnabled(const CGameScriptInterface* self)
{
    return self->World->flag;
}

int main()
{
    CWorld a; a.flag = 5;
    CWorld b; b.flag = 0;
    CGameScriptInterface s;
    s.World = &a;
    if (CGameScriptInterface_IsSavingEnabled(&s) != 5) { std::printf("FAIL nonzero\n"); return 1; }
    s.World = &b;
    if (CGameScriptInterface_IsSavingEnabled(&s) != 0) { std::printf("FAIL zero\n"); return 1; }
    std::printf("CGameScriptInterface_0088f950_TEST PASS\n");
    return 0;
}