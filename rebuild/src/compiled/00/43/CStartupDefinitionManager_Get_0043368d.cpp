// CStartupDefinitionManager::Get, retail 0x0043368d (FableWin ?Get@CStartupDefinitionManager@@SIAAV1@XZ): the singleton behind CStartupDefinitionManager::PClass.
#include "fable_definition_manager.h"

CStartupDefinitionManager& CStartupDefinitionManager::Get()
{
    return *PClass.Get();
}