// The static data's initial values as code reads them (the inline accessors
// retail expands), linked against the provider only.
#include "fable_game.h"
#include <stdio.h>
int main()
{
    if (CMainGameComponent::GetConstantFPS() != 15) return 1;
    if (CMainGameComponent::AlwaysUpdateGameFrame) return 2;
    if (CMainGameComponent::ProfileUpdateTime != 0 || CMainGameComponent::ProfileRenderTime != 0) return 3;
    if (CMainGameComponent::Get() != 0) return 4;
    printf("MAIN_GAME_STATIC_DATA_PASS\n");
    return 0;
}
