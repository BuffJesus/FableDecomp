// CDefinitionManager::GetPDefClassInfo(const CCharString&) const @ 0x009AD2E0
// (FableWin ?GetPDefClassInfo@CDefinitionManager@@IBEPBVCDefClassInfo@1@ABVCCharString@@@Z;
// callers AddDefClass 0x009B0AC0, GetPDefFromClassIndex 0x009ADE10). The
// non-const overload 0x009AD580 is the same body over the other LowerBound.
#include "fable_definition_manager.h"

// The class info registered under the CRC of the class name, or null.
const CDefinitionManager::CDefClassInfo* CDefinitionManager::GetPDefClassInfo(const CCharString& className) const
{
    const _STL::pair<unsigned long, CDefClassInfo>* it = DefClassInfo.Find(className.GetCRC());
    if (it == DefClassInfo.end())
        return 0;
    return &it->second;
}