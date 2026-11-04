#include "fable_definition_manager.h"
#include "fable_def_class_base.h"
CDefPointer<CDefClassBase> CDefinitionManager::GetPDefFromInstantiationName(const CCharString& name)
{
    long index=GetDefGlobalIndexFromName(name);
    if(index>0)
    {
        LoadDef(index);
        return InstantiatedDefs[index]->PDef;
    }
    return CDefPointer<CDefClassBase>();
}
