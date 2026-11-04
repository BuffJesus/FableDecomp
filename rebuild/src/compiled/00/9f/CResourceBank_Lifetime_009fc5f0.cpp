// Shared resource lifetimes. Size optimization selects the retail resource
// deleting wrapper; list and bank bodies retain their speed-optimized form.
#pragma optimize("s", on)
#include "fable_game_text_bank.h"

CResource::CResource()
    : ResourceList(0), PrevResource(0), NextResource(0), ResourceSize(0), LastUsedFrame(0) {}

#pragma optimize("", on)
inline CResourceList::CResourceList()
{
    Head.PrevResource = &Head;
    Head.NextResource = &Head;
    ResourceCount = 0;
    CurrentFrame = 0;
    DebugStatsFrame = 0;
    AllocatedMemory = 0;
    MaximumMemory = 0x7fffffff;
    UnloadDelay = 1;
    UnloadedThisFrame = 0;
}
CResourceBank::CResourceBank() {}
inline CResourceList::~CResourceList() {}
CResourceBank::~CResourceBank() {}
