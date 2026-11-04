// CMainGameComponent::PostChangeResolution @ 0x00416365 (MainGameComponent.cpp is size-optimised).
#pragma optimize("s", on)
#include "fable_game.h"
#include "fable_display_engine.h"
#include "fable_world.h"
void CMainGameComponent::PostChangeResolution(unsigned long width, unsigned long height, unsigned long depth)
{
    PDisplayEngine->PostChangeResolution(width, height, depth);
    PWorld->ChangeResolution(width, height, depth);
}
