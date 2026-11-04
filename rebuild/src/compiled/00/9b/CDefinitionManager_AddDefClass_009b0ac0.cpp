// CDefinitionManager::AddDefClass(CDefClassInit const&) @ 0x009B0AC0 (retail
// DefinitionManager.cpp; callers CNewFrontendGameComponent::CompileDefsPlatformSpecific
// 0x0042F5A9 and the game-definition setup). The unit also defines the two
// static switches it reads (retail 0x013CA7D8 and 0x0138E189, zero at load).
#include "fable_definition_manager.h"
#include "fable_def_class_base.h"
#include "fable_persist_context.h"
#include "fable_progress_display.h"

bool CDefinitionManager::UsingCompiledDefs;
bool CDefinitionManager::CreateChecksums;

// Register a definition class under the CRC of its name. When the defs are
// checksummed or compiled from text, instantiate the class's NULLDEF_ default
// right away and give it its default values.
void CDefinitionManager::AddDefClass(const CDefClassInit& init)
{
    NProgressDisplay::DisplayProgress("Add Def Class", -1.0f, false, false);

    CDefClassInfo info;
    info.AllocFunc = init.AllocFunc;
    info.Name = init.Name;
    DefClassInfo.Insert(init.Name.GetCRC(), info);
    DefClassInfo.Update();

    if (CreateChecksums || !UsingCompiledDefs)
    {
        CCharString nullDefName = "NULLDEF_" + init.Name;
        InstantiateDef(init.Name, nullDefName, false);
        // The class's first global index, read straight off the array: an
        // inlined operator[] leaves a dead `this` add VC7.1 does not remove.
        unsigned long globalIndex = GetPDefClassInfo(init.Name)->GlobalIndices._M_start[0];
        LoadDef(globalIndex);
        CDefPointer<CDefClassBase> def = InstantiatedDefs[globalIndex]->PDef;
        CPersistContext context;
        context.InitClearMode();
        def->Transfer(context);
    }
}