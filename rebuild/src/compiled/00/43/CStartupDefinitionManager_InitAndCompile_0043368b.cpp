#pragma optimize("s", on)
// CStartupDefinitionManager::InitAndCompile @ 0x0043368B (vtable 0x0123117C slot 2).
#include "fable_definition_manager.h"

// The front end compiles per platform (InitAndCompilePlatformSpecific), so the
// generic virtual hook does nothing.
void CStartupDefinitionManager::InitAndCompile()
{
}
