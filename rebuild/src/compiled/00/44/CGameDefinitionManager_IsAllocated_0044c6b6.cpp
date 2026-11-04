#pragma optimize("s", on)
// CGameDefinitionManager::IsAllocated, retail 0x0044C6B6
// (FableWin ?IsAllocated@CGameDefinitionManager@@SI_NXZ): has the singleton been created?
#include "fable_definition_manager.h"

bool CGameDefinitionManager::IsAllocated()
{
    return !PClass.IsNull();
}