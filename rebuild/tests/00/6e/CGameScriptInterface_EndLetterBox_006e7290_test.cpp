#include "engine/CGameScriptInterface.h"
#include <cstdio>
struct CDisplayEngine { int val; void EndLetterBox(){ val = 42; } };
static void __fastcall CGameScriptInterface_EndLetterBox(CGameScriptInterface* self)
{
    self->DisplayEngine->EndLetterBox();
}
int main(){
    CDisplayEngine inner; inner.val = 0;
    CGameScriptInterface obj; obj.DisplayEngine = &inner;
    CGameScriptInterface_EndLetterBox(&obj);
    if (inner.val == 42) { std::printf("CGameScriptInterface_006e7290_TEST PASS\n"); return 0; }
    std::printf("FAIL\n"); return 1;
}