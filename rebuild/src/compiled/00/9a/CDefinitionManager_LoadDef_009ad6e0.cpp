#include "fable_definition_manager.h"
#include "fable_def_class_base.h"
#include "fable_data_input_stream.h"
#include "fable_persist_context.h"
#include "fable_progress_display.h"

void CDefinitionManager::LoadDef(unsigned long globalIndex) const
{
    CInstantiatedDefInfo* info = InstantiatedDefs[globalIndex];
    if (info->PDef.Get())
    {
        info->PDef->TouchResource();
        return;
    }
    NProgressDisplay::DisplayProgress("CDefinitionManager::LoadDef 1", -1.0f, false, false);
    CMemoryDataInputStream stream;
    DecompressDefinitionData(stream, globalIndex);
    CCharString className = static_cast<CCharString>(info->ClassName);
    const CDefClassInfo* classInfo = GetPDefClassInfo(className);
    if (classInfo)
    {
        unsigned long resourceSize = stream.GetLength() + sizeof(CDefClassBase);
        CDefinitionManager* manager = const_cast<CDefinitionManager*>(this);
        manager->MakeFreeSpace(resourceSize);
        manager->FrameEnd();
        info->PDef.Reset(classInfo->AllocFunc());
        info->PDef->SetInstantiationName(info->InstantiationName);
        info->PDef->PDefManager = manager;
        info->PDef->GlobalIndex = globalIndex;
        info->PDef->InternalLoadBinary(stream);
        NProgressDisplay::DisplayProgress("CDefinitionManager::LoadDef 2", -1.0f, false, false);
        CPersistContext context;
        context.Init(stream, UseSafeBinaryFormat);
        info->PDef->Transfer(context);
        info->PDef->SetResourceSize(resourceSize);
        manager->AddResource(*info->PDef);
        info->PDef->TouchResource();
        NProgressDisplay::DisplayProgress("CDefinitionManager::LoadDef 3", -1.0f, false, false);
    }
}
