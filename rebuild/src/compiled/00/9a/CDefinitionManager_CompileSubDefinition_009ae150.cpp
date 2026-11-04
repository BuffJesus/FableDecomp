#include "fable_definition_manager.h"
#include "fable_string_parser.h"
#include "fable_persist_context.h"
#include "fable_def_class_base.h"

void CDefinitionManager::CompileSubDefinition(CCharString source, const CCharString& name, long globalIndex)
{
    LoadDef(globalIndex);
    CDefPointer<CDefClassBase> parent = InstantiatedDefs[globalIndex]->PDef;
    CDefClassBase::CSubDefInfo* info = parent->GetPSubDefInfo(name);
    CDefPointer<CDefClassBase> target;
    bool useDefaults;
    if (info)
    {
        if (info->OriginalParentDefIndex != globalIndex)
        {
            long originalIndex = info->DefIndex;
            LoadDef(originalIndex);
            CDefPointer<CDefClassBase> original = InstantiatedDefs[originalIndex]->PDef;
            parent->RemoveSubDef(name);
            target = InstantiateSubDef(name, globalIndex);
            target->Copy(original.Get());
        }
        else
        {
            LoadDef(info->DefIndex);
            target = InstantiatedDefs[info->DefIndex]->PDef;
        }
        useDefaults = false;
    }
    else
    {
        target = InstantiateSubDef(name, globalIndex);
        useDefaults = true;
    }
    if (target.Get())
    {
        CStringParser parser(source, ErrorCallback, "", 0);
        CPersistContext context(parser, ErrorCallback, useDefaults, this, PSymbolMap.Get(), &source);
        target->Transfer(context);
        target->Setup = true;
        target->DefaultValsApplied = true;
        context.CheckUnknownSymbols();
    }
}
