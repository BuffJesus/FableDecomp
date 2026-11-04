#include <new>
#include "fable_definition_manager.h"
#include "fable_def_class_base.h"
#include "fable_stlport_vector_impl.h"

CDefPointer<CDefClassBase> CDefinitionManager::InstantiateSubDef(const CCharString& name, unsigned long parentIndex)
{
    CDefClassInfo* info = GetPDefClassInfo(name);
    if (!info)
    {
        LoadDef(parentIndex);
        CDefPointer<CDefClassBase> parent = InstantiatedDefs[parentIndex]->PDef;
        return CDefPointer<CDefClassBase>();
    }
    unsigned long globalIndex = InstantiatedDefs.size();
    unsigned long classIndex = info->GlobalIndices.size();
    CDefPointer<CDefClassBase> child(info->AllocFunc());
    child->Template = false;
    child->PDefManager = this;
    child->GlobalIndex = globalIndex;
    CInstantiatedDefInfo* instantiated = new CInstantiatedDefInfo(CDefString(info->Name), CDefString(), classIndex, child);
    InstantiatedDefs.push_back(instantiated);
    info->GlobalIndices.push_back(globalIndex);
    LoadDef(parentIndex);
    CDefPointer<CDefClassBase> parent = InstantiatedDefs[parentIndex]->PDef;
    parent->AddSubDef(info->Name, globalIndex, parentIndex);
    return child;
}
