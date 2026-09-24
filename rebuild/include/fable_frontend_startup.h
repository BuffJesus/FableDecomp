#pragma once
#include "fable_game.h"

// External dependencies of the recovered frontend startup functions. This
// declaration describes the called ABI only; console ownership is not linked.
class CConsole
{
public:
    // ToggleChar / ToggleKey / PFont in the donor layout; EInputKey is 32-bit.
    void Initialise(char toggleCharacter, int toggleKey, CFontBank* font);
};
extern CNewFrontendGameComponent* g_FableNewFrontend_013B871C;
extern void FABLE_CDECL StaticLoadXMVCode();
extern CConsole* FABLE_CDECL FableGetConsole_00414C90();
extern double FABLE_CDECL GFGetTime();
