#include "fable_definition_manager.h"
#include "fable_def_class_base.h"
#include "fable_symbol_map.h"

void CDefinitionManager::RemoveUnusedMemory()
{
    reinterpret_cast<CArrayVector<CInstantiatedDefInfo*>&>(InstantiatedDefs)
        .DeallocateUnusedMemory();
    PDefinitionSymbolMap.Data->DeallocateUnusedMemory();

    for (unsigned long i = 0; i < DefClassInfo.size(); ++i)
    {
        reinterpret_cast<CArrayVector<unsigned long>&>(DefClassInfo[i].second.GlobalIndices)
            .DeallocateUnusedMemory();
    }

    for (unsigned long i = 0; i < InstantiatedDefs.size(); ++i)
    {
        if (InstantiatedDefs[i]->PDef.Get()->GetPSubDefInfoMap())
        {
            CParentDefClassBase::CSubDefInfoMap* children =
                static_cast<CParentDefClassBase::CSubDefInfoMap*>(
                    InstantiatedDefs[i]->PDef.Get()->GetPSubDefInfoMap());
            children->DeallocateUnusedMemory();
        }
    }
}
