#pragma optimize("s", on)
// CStartupDefinitionManager::CStartupDefinitionManager(CWideString const&) @ 0x00433693.
#include "fable_definition_manager.h"

// The front end's definition manager over the given .bin, reporting compile
// errors through its (empty) ErrorCallback.
CStartupDefinitionManager::CStartupDefinitionManager(const CWideString& filename)
    : CDefinitionManager(filename, ErrorCallback)
{
}