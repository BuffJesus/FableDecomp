// CMainGameComponent::SignalFrameUpdate @ 0x00416047 (15 bytes).
// FableWin ?SignalFrameUpdate@CMainGameComponent@@QAEXXZ. Clears
// FirstWorldFrameUpdate and starts a new allocator frame.
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_memory_allocator.h"

void CMainGameComponent::SignalFrameUpdate()
{
    FirstWorldFrameUpdate = false;
    CMemoryAllocator::Get()->NewFrame();
}
