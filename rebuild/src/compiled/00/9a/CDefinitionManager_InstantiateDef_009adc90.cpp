#include "fable_definition_manager.h"
#include "fable_def_class_base.h"
#include "fable_stlport_vector_impl.h"
#include <new>

bool CDefinitionManager::InstantiateDef(const CCharString& className,
                                       const CCharString& instanceName, bool isTemplate)
{
    CDefClassInfo* info = GetPDefClassInfo(className);
    if (!info) return false;
    unsigned long globalIndex = InstantiatedDefs.size();
    unsigned long classIndex = info->GlobalIndices.size();
    CDefPointer<CDefClassBase> def(info->AllocFunc());
    def->SetAsTemplate(isTemplate);
    if (def->IsParentDef()) def->SetInstantiationName(CDefString(instanceName));
    def->PDefManager = this;
    def->GlobalIndex = globalIndex;
    CInstantiatedDefInfo* instance = new CInstantiatedDefInfo(CDefString(info->Name),
        CDefString(instanceName), classIndex, def);
    InstantiatedDefs.push_back(instance);
    info->GlobalIndices.push_back(globalIndex);
    PSymbolMap.Data->AddSymbolDirty(instanceName, globalIndex);
    PDefinitionSymbolMap.Data->AddSymbolDirty(info->Name, instanceName, globalIndex);
    return true;
}
