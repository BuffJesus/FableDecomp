// CMainGameComponent::InitLUA @ 0x004BBC00 (3 bytes).
// FableWin ?InitLUA@CMainGameComponent@@IAEXABVCCharString@@@Z. Empty in the
// retail build (`ret 4`). The linker folded it with the other empty
// one-argument thiscall bodies at 0x004BBC00; Ego_r's copy 0x004BF6D0 is the
// same `ret 4`, also folded away from MainGameComponent.cpp. Init 0x004184BD
// calls it.
#include "fable_game.h"

void CMainGameComponent::InitLUA(const CCharString&)
{
}
