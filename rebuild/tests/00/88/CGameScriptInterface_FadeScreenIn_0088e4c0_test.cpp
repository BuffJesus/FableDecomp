#include "engine/CGameScriptInterface.h"
#include <cstdio>
struct CDisplayEngine { char pad[0xd8]; unsigned char flag; };
static int g_called = 0;
void __fastcall CGameScriptInterface_DoFade(CGameScriptInterface* self){ g_called = 1; }
void __fastcall CGameScriptInterface_FadeScreenIn(CGameScriptInterface* self)
{
    self->DisplayEngine->flag = 0;
    CGameScriptInterface_DoFade(self);
}
int main(){
    CDisplayEngine subObj; subObj.flag = 0xAB;
    CGameScriptInterface obj; obj.DisplayEngine = &subObj;
    CGameScriptInterface_FadeScreenIn(&obj);
    if (subObj.flag == 0 && g_called == 1){ std::printf("CGameScriptInterface_0088e4c0_TEST PASS\n"); return 0; }
    std::printf("FAIL flag=%d called=%d\n", subObj.flag, g_called); return 1;
}