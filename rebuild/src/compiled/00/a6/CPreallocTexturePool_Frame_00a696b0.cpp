#include "engine/CPreallocTexturePool.h"
void CPreallocTexturePool::FrameEnd(){CResourceList::FrameEnd();}
unsigned long CPreallocTexturePool::GetTextureCount(){return Count;}
