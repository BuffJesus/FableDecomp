#include "fable_definition_manager.h"
#include "fable_def_class_base.h"

CDefPointer<CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long globalIndex)
{
    LoadDef(globalIndex);
    return InstantiatedDefs[globalIndex]->PDef;
}
