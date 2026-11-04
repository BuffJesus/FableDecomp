#include "fable_resource_list.h"
// Standalone native entry point; the resource-bank TU keeps an inline copy.
void CResourceList::FrameEnd(){++CurrentFrame;UnloadedThisFrame=0;}
