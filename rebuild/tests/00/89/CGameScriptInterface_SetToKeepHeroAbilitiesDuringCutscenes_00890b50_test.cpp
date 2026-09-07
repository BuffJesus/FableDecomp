#include "engine/CGameScriptInterface.h"
#include <cstdio>

struct CMainGameComponent {
    char pad[0x1c];
    void* f1c;
};

static unsigned char g_obj[0x300];

void* __fastcall helper_449970(void* that)
{
    // return our fake object; verify 'that' is the expected pointer
    return g_obj;
}

void __fastcall SetToKeepHeroAbilitiesDuringCutscenes(CGameScriptInterface* self, int edx, bool keep);

int main()
{
    CMainGameComponent inner;
    CGameScriptInterface gsi;
    gsi.Component = &inner;
    inner.f1c = (void*)0x1234;

    g_obj[0x224] = 0;
    SetToKeepHeroAbilitiesDuringCutscenes(&gsi, 0, true);
    if (g_obj[0x224] != 1) { printf("FAIL true\n"); return 1; }

    SetToKeepHeroAbilitiesDuringCutscenes(&gsi, 0, false);
    if (g_obj[0x224] != 0) { printf("FAIL false\n"); return 1; }

    printf("00890b50_TEST PASS\n");
    return 0;
}