#include <stdio.h>

#include "candidates/game_script_interface_screen_effect_compiled.h"

int main()
{
    char storage[sizeof(CGameScriptInterface)] = { 0 };
    CGameScriptInterface* scripts =
        reinterpret_cast<CGameScriptInterface*>(storage);
    scripts->CGameScriptInterface::CameraCancelScreenEffect();
    printf("CAMERA_CANCEL_SCREEN_EFFECT_TEST PASS\n");
    return 0;
}
