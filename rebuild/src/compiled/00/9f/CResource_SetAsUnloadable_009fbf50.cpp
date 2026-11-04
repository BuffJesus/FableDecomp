// CResource::SetAsUnloadable() @ 0x009FBF50 (Ego_r/FableWin ?SetAsUnloadable@CResource@@QAEXXZ).
// Back-date the last use so the resource is already past the list's unload delay.
#include "fable_resource_list.h"

void CResource::SetAsUnloadable()
{
    if (ResourceList)
        LastUsedFrame = ResourceList->CurrentFrame - ResourceList->UnloadDelay;
}
