// CDefinitionManager::GetPDefClassInfo(const CCharString&) @ 0x009AD580
// (retail DefinitionManager.cpp; caller InstantiateDef 0x009ADC90). The
// byte-identical 0x009AD2E0 goes through the const LowerBound instead.
#include "fable_definition_manager.h"

// The class info registered under the CRC of the class name, or null.
CDefinitionManager::CDefClassInfo* CDefinitionManager::GetPDefClassInfo(const CCharString& className)
{
    _STL::pair<unsigned long, CDefClassInfo>* it = DefClassInfo.Find(className.GetCRC());
    if (it == DefClassInfo.end())
        return 0;
    return &it->second;
}