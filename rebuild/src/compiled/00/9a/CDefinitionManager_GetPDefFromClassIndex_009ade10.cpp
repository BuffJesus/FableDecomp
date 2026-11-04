// CDefinitionManager::GetPDefFromClassIndex(const CCharString&, long) const
// @ 0x009ADE10 (FableWin
// ?GetPDefFromClassIndex@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@ABVCCharString@@J@Z;
// the manifest labels it GetPDefFromGlobalIndex). Callers: every GetDef<T>
// (0x004300FB for CFrontEndDef).
#include "fable_definition_manager.h"

// The class's index-th def: map it to its global index, make sure it is
// loaded (LoadDef 0x009AD6E0) and share it (the returned pointer holds a
// lock). The class info is a named local: indexing the call's result
// directly leaves a dead `add eax, 8` (the vector's this) in VC7.1's output.
CDefPointer<const CDefClassBase> CDefinitionManager::GetPDefFromClassIndex(const CCharString& className, long index) const
{
    const CDefClassInfo* info = GetPDefClassInfo(className);
    unsigned long globalIndex = info->GlobalIndices[index];
    LoadDef(globalIndex);
    return InstantiatedDefs[globalIndex]->PDef;
}