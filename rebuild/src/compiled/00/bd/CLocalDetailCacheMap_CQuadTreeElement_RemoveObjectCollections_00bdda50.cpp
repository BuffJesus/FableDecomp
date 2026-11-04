#include "engine/CLocalDetailCacheMap.h"
void CLocalDetailCacheMap::CQuadTreeElement::RemoveObjectCollections() {
    while(ObjectCacheGroupCollections) {
        CObjectCacheGroupCollection* next=ObjectCacheGroupCollections->Next;
        delete ObjectCacheGroupCollections;
        ObjectCacheGroupCollections=next;
    }
}
