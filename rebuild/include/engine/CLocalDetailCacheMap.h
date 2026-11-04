#pragma once
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_carray.h"
#include "engine/C2DBoxI.h"
#include "fable_movable_resource.h"
#include "fable_bounding_volumes.h"
class CDataInputStream;
class CEngineStaticMapFileBlockManager;
class CLocalDetailObjectCollectionType;
class CEngineMap;
// Retail layout; allocation size0x70 is verified by CreateMap at00B59800.
class CLocalDetailCacheMap
{
public:
    explicit CLocalDetailCacheMap(CEngineMap*);
    struct CFileHeader {
        long DataFilePos,DataSize;
        bool HasQuadTree;
        char Pad[3];
        CFileHeader() : DataFilePos(0),DataSize(0),HasQuadTree(false) {}
    };
    ~CLocalDetailCacheMap();
class CObjectTypeCollectionPalette
{
public:
    void Load(CDataInputStream& stream);
    CArrayVector<const CLocalDetailObjectCollectionType*> Entries;
};
class CObjectTypeCollection : public CMovableResource
{
public:
    const CLocalDetailObjectCollectionType* Type;
    void* Primitives;
    CObjectTypeCollection* Next;
};
class CObjectCacheGroupCollection : public CMovableResource
{
public:
    virtual ~CObjectCacheGroupCollection();
    CObjectCacheGroupCollection* Next;
    float BoundingSphere[4];
    float MaxFadeDistance;
    unsigned long PrimitiveTypeMask;
    CObjectTypeCollection* ObjectTypeCollections;
};
class CQuadTreeElement : public CMovableResource
{
public:
    struct CFileHeader {
        C3DBoundingSphere BoundingSphere;
        float MaxFadeDistance;
        unsigned long PrimitiveTypeMask;
        long FileBlockPos;
        unsigned long FileBlockSize,OffsetIntoFileBlock;
        unsigned short MapX,MapY,Width,Height;
    };
    CQuadTreeElement(CLocalDetailCacheMap*,long,long,long,long);
    virtual ~CQuadTreeElement();
    virtual void RelocateData(long,void*,void*,long);
    virtual bool CanRelocateData(long) const;
    void OnPostRelocation();
    static void operator delete(void*);
    CObjectCacheGroupCollection* ObjectCacheGroupCollections;
    CQuadTreeElement* Children[4];
    C3DBoundingSphere BoundingSphere;
    CLocalDetailCacheMap* Map;
    float MaxFadeDistance;
    unsigned long PrimitiveTypeMask;
    long FileBlockPos;
    unsigned long FileBlockSize;
    unsigned long OffsetIntoFileBlock;
    union { struct { unsigned short MapX,MapY,Width,Height; }; fable_u8 Coordinates[8]; };
    unsigned char StateFlags;
    fable_u8 TailPadding[3];
    void LoadHeader(CDataInputStream& stream);
    void UnloadStaticContents(CEngineStaticMapFileBlockManager*);
    void RemoveObjectCollections();
    void Clear(CEngineStaticMapFileBlockManager*);
};

    bool LockedFromMemoryRelocation;
    fable_u8 pad_01[3];
    CEngineStaticMapFileBlockManager* StaticMapFileBlockManager;
    CEngineMap* OwnerMap;
    CQuadTreeElement QuadTreeElement;
    _STL::vector<C2DBoxI> AreaUpdateList;
    CObjectTypeCollectionPalette ObjectTypeCollectionPalette;

    void OnMapVisibilityChanged(bool);
    void CloseStaticMap();
    void OpenStaticMap(CEngineStaticMapFileBlockManager* manager, CDataInputStream& stream);
};

typedef CLocalDetailCacheMap::CQuadTreeElement CQuadTreeElement;
typedef CLocalDetailCacheMap::CObjectTypeCollectionPalette CObjectTypeCollectionPalette;
FABLE_STATIC_ASSERT(sizeof(CQuadTreeElement)==0x4c);
FABLE_STATIC_ASSERT(offsetof(CQuadTreeElement,Children)==8);
FABLE_STATIC_ASSERT(offsetof(CQuadTreeElement,FileBlockPos)==0x34);
FABLE_STATIC_ASSERT(offsetof(CQuadTreeElement,StateFlags)==0x48);

FABLE_STATIC_ASSERT(offsetof(CLocalDetailCacheMap,StaticMapFileBlockManager)==4);
FABLE_STATIC_ASSERT(offsetof(CLocalDetailCacheMap,QuadTreeElement)==0xc);
FABLE_STATIC_ASSERT(offsetof(CLocalDetailCacheMap,ObjectTypeCollectionPalette)==0x64);
