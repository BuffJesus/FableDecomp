#include <stdio.h>

#include "candidates/game_script_interface_screen_effect_compiled.h"

int main()
{
    char storage[sizeof(CGameScriptInterface)] = { 0 };
    CGameScriptInterface* scripts =
        reinterpret_cast<CGameScriptInterface*>(storage);
    scripts->CGameScriptInterface::CameraUseScreenEffect(0.75f, 1.5f, 2.25f);
    printf("CAMERA_USE_SCREEN_EFFECT_TEST PASS\n");
    return 0;
}
