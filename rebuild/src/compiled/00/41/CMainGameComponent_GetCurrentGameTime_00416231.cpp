#include "fable_game.h"
#pragma optimize("s", on)
double CMainGameComponent::GetCurrentGameTime() const { return GFGetTime() - GameStartTime; }
