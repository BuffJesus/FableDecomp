#include "fable_definition_manager.h"
#include "fable_def_class_base.h"

CDefPointer<const CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long globalIndex) const
{
    LoadDef(globalIndex);
    return InstantiatedDefs[globalIndex]->PDef;
}
