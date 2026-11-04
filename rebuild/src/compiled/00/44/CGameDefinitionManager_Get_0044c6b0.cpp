// CGameDefinitionManager::Get, retail 0x0044c6b0 (FableWin ?Get@CGameDefinitionManager@@SIAAV1@XZ): the singleton behind CGameDefinitionManager::PClass.
#include "fable_definition_manager.h"

CGameDefinitionManager& CGameDefinitionManager::Get()
{
    return *PClass.Get();
}