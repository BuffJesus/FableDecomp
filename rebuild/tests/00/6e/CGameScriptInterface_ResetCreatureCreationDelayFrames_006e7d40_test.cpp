#include "engine/CGameScriptInterface.h"
#include <cstdio>


void __fastcall CGameScriptInterface_ResetCreatureCreationDelayFrames(CGameScriptInterface* self)
{
    self->CameraResetToViewBehindHeroCount = 0;
}

int main()
{
    CGameScriptInterface obj;
    obj.CameraResetToViewBehindHeroCount = 12345;
    CGameScriptInterface_ResetCreatureCreationDelayFrames(&obj);
    if (obj.CameraResetToViewBehindHeroCount != 0) {
        std::printf("FAIL: expected 0 got %d\n", obj.CameraResetToViewBehindHeroCount);
        return 1;
    }
    std::printf("CGameScriptInterface_006e7d40_TEST PASS\n");
    return 0;
}