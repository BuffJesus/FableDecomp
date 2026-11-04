// CResourceList::CResourceList() @ 0x009FC570, and CResource::CResource()
// @ 0x009FBEC0 (both in retail's ResourceList.cpp: the compiler expands the
// resource ctor into the list's for its Head sentinel).
#include "fable_resource_list.h"

// Not on a list, unsized, never used; one reference.
CResource::CResource()
    : ResourceList(0), PrevResource(0), NextResource(0), ResourceSize(0), LastUsedFrame(0)
{
}

// An empty list: the sentinel links to itself, no memory cap, resources
// unload one frame after their last use.
CResourceList::CResourceList()
{
    Head.PrevResource = &Head;
    Head.NextResource = &Head;
    ResourceCount = 0;
    CurrentFrame = 0;
    DebugStatsFrame = 0;
    AllocatedMemory = 0;
    MaximumMemory = 0x7FFFFFFF;
    UnloadDelay = 1;
    UnloadedThisFrame = 0;
}
