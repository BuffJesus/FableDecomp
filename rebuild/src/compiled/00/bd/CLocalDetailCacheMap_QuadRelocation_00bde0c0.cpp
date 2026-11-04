#include "engine/CLocalDetailCacheMap.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineLandscapeMemoryManager.h"
#include "fable_engine_internal.h"
template<class T> static inline void RelocatePointer(T*& p,void* oldp,void* newp) {
    if(p)p=reinterpret_cast<T*>(reinterpret_cast<unsigned long>(p)+reinterpret_cast<unsigned long>(newp)-reinterpret_cast<unsigned long>(oldp));
}
void CLocalDetailCacheMap::CQuadTreeElement::RelocateData(long,void* oldp,void* newp,long) {
    for(long i=0;i<4;++i) {
        if(Children[i]==oldp) {
            RelocatePointer(Children[i],oldp,newp);
            Children[i]->OnPostRelocation();
            return;
        }
    }
    CObjectCacheGroupCollection* previous=0;
    CObjectCacheGroupCollection* current=ObjectCacheGroupCollections;
    while(current) {
        if(current==oldp) {
            RelocatePointer(current,oldp,newp);
            if(previous)previous->Next=current;
            else ObjectCacheGroupCollections=current;
            CObjectTypeCollection* collection=current->ObjectTypeCollections;
            CEngineLandscapeMemoryManager* manager=NEngineInternal::GPLandscapeRenderer->MemoryManager;
            while(collection) {
                manager->SetVirtualBlockMovableResource(collection,current);
                collection=collection->Next;
            }
            return;
        }
        previous=current;
        current=current->Next;
    }
}
void CLocalDetailCacheMap::CQuadTreeElement::OnPostRelocation() {
    CEngineLandscapeMemoryManager* manager=NEngineInternal::GPLandscapeRenderer->MemoryManager;
    for(long i=0;i<4;++i)
        if(Children[i])manager->SetVirtualBlockMovableResource(Children[i],this);
    CObjectCacheGroupCollection* current=ObjectCacheGroupCollections;
    while(current) {
        manager->SetVirtualBlockMovableResource(current,this);
        current=current->Next;
    }
}
