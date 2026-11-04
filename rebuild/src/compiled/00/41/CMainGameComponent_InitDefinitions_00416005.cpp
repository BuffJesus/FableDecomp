// CMainGameComponent::InitDefinitions @ 0x00416005 (66 bytes).
// FableWin ?InitDefinitions@CMainGameComponent@@IAE_N_N@Z. Runs the game
// definition manager's InitAndCompile (vtable slot 2), clears the compiler's
// symbol table when compiling, and returns true. The allocator totals it
// samples before and after only fed a debug print that retail compiles out;
// the out-of-line GetTotalMemoryAllocated calls remain.
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_definition_manager.h"
#include "fable_memory_allocator.h"

bool CMainGameComponent::InitDefinitions(bool compile)
{
    unsigned long memoryBefore = 0;
    if (CMemoryAllocator::Get())
        memoryBefore = CMemoryAllocator::Get()->GetTotalMemoryAllocated();

    CGameDefinitionManager::Get().InitAndCompile();
    if (compile)
        CGameDefinitionManager::Get().ClearSymbols();

    unsigned long memoryAfter = 0;
    if (CMemoryAllocator::Get())
        memoryAfter = CMemoryAllocator::Get()->GetTotalMemoryAllocated();

    return true;
}
