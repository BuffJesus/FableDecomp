#include "engine/CLocalDetailCacheMap.h"
#include "engine/CEngineStaticMapFileBlockManager.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineLandscapeMemoryManager.h"
#include "fable_engine_internal.h"
bool CMovableResource::CanRelocateData(long) const { return true; }
void CLocalDetailCacheMap::CQuadTreeElement::operator delete(void* p) {
    NEngineInternal::GPLandscapeRenderer->MemoryManager->FreeVirtual(p);
}
bool CLocalDetailCacheMap::CQuadTreeElement::CanRelocateData(long) const {
    return !Map->LockedFromMemoryRelocation;
}
CLocalDetailCacheMap::CQuadTreeElement::~CQuadTreeElement() {
    if(StateFlags&8) {
        Map->StaticMapFileBlockManager->ReleaseRequest(FileBlockPos);
        StateFlags&=~8;
    }
    RemoveObjectCollections();
    delete Children[0];
    delete Children[1];
    delete Children[2];
    delete Children[3];
}
