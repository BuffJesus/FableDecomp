#ifndef FABLETLC_LANDSCAPE_PATCH_LOAD_FOREGROUND_COMPILED_H
#define FABLETLC_LANDSCAPE_PATCH_LOAD_FOREGROUND_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

enum LS_LOADING_STATUS
{
    LS_LOADING_READY = 1,
    LS_LOADING_PENDING = 2,
    LS_LOADING_FAILED = 3
};

struct CForegroundCountedReference
{
    long Count;
    void (*Release)(void* owner);
    void* Owner;
};

template<class T>
class CForegroundCountedPointer
{
public:
    T* Pointer;
    CForegroundCountedReference* Reference;

    CForegroundCountedPointer() : Pointer(0), Reference(0) {}
    CForegroundCountedPointer(T* pointer, CForegroundCountedReference* reference)
        : Pointer(pointer), Reference(reference)
    {
        if (Reference != 0)
            ++Reference->Count;
    }
    CForegroundCountedPointer(const CForegroundCountedPointer& other)
        : Pointer(other.Pointer), Reference(other.Reference)
    {
        if (Reference != 0)
            ++Reference->Count;
    }
    ~CForegroundCountedPointer() { Clear(); }
    CForegroundCountedPointer& operator=(const CForegroundCountedPointer& other)
    {
        if (this != &other)
        {
            Clear();
            Pointer = other.Pointer;
            Reference = other.Reference;
            if (Reference != 0)
                ++Reference->Count;
        }
        return *this;
    }
    void Clear()
    {
        if (Reference != 0 && --Reference->Count == 0)
        {
            Reference->Release(Reference->Owner);
            operator delete(Reference);
        }
        Pointer = 0;
        Reference = 0;
    }
};

class CBankFileAsyncData
{
public:
    bool IsFinished();
    const fable_u8* GetData();
};

class CBankFileAsyncEntry
{
public:
    CForegroundCountedPointer<CBankFileAsyncData> BeginReadDataAutoAlloc(
        unsigned long offset,
        unsigned long size);
};

class CMemoryDataInputStream
{
public:
    const fable_u8* Data;
    unsigned long Size;
    unsigned long Position;

    CMemoryDataInputStream(const void* data, unsigned long size);
    bool Read8(fable_u8& value);
    bool Read16(fable_u16& value);
    bool Read32(fable_u32& value);
};

class CEngineTexturePalette {};

class CEngineLandscapeMap
{
public:
    fable_u8 Unknown00[0x40];
    CEngineTexturePalette TexturePalette;
};

class CLandscapeLayerMesh
{
public:
    fable_u8 Unknown00[4];
    fable_i32 ForegroundTextureIndex;
    fable_u8 Unknown08[4];
    fable_i32 BumpMapTextureIndex;
    fable_u8 Unknown10[0x2C];
    CLandscapeLayerMesh* Next;

    bool LoadForeground(CMemoryDataInputStream& input, CEngineTexturePalette& palette);
};

class CWaterPatchMesh {};
class CEngineLandscapePatch;

CLandscapeLayerMesh* CreateLandscapeLayerMesh(CEngineLandscapeMap* map);
void DestroyLandscapeLayerMesh(CLandscapeLayerMesh* layer);
void* AllocateLandscapePatchTextureIDs(unsigned long bytes, CEngineLandscapePatch* owner);
bool DecompressLandscapeForeground(
    const fable_u8* source,
    unsigned long sourceSize,
    fable_u8* destination,
    unsigned long* destinationSize);
CWaterPatchMesh* CreateWaterPatchMesh(
    CEngineLandscapeMap* map,
    CMemoryDataInputStream& input,
    fable_u32 x,
    fable_u32 y);
void DestroyWaterPatchMesh(CWaterPatchMesh* water);

class CEngineLandscapePatch
{
public:
    fable_u8 Unknown00[4];
    CEngineLandscapeMap* Map;
    CLandscapeLayerMesh* LayerMeshList;
    fable_u16* FastAccessTextureIDs;
    fable_u16 FastAccessTextureIDCount;
    fable_u16 Unknown12;
    CWaterPatchMesh* WaterPatch;
    fable_u32 FilePosition;
    fable_i32 ForegroundDataSize;
    fable_u8 Bounds[0x18];
    fable_u16 MapX;
    fable_u16 MapY;
    fable_u8 Flags;
    fable_u8 Unknown3D[3];
    CForegroundCountedPointer<CBankFileAsyncData> LoadingPatch;

    LS_LOADING_STATUS LoadForegroundPatch(
        CForegroundCountedPointer<CBankFileAsyncEntry> bankEntry);
};

FABLE_STATIC_ASSERT(sizeof(CForegroundCountedPointer<CBankFileAsyncData>) == 8);
FABLE_STATIC_ASSERT(sizeof(CLandscapeLayerMesh) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CLandscapeLayerMesh, ForegroundTextureIndex) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CLandscapeLayerMesh, BumpMapTextureIndex) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CLandscapeLayerMesh, Next) == 0x3C);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, Map) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, LayerMeshList) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, FastAccessTextureIDs) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, FastAccessTextureIDCount) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, WaterPatch) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, FilePosition) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, ForegroundDataSize) == 0x1C);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, Flags) == 0x3C);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, LoadingPatch) == 0x40);
FABLE_STATIC_ASSERT(sizeof(CEngineLandscapePatch) == 0x48);

#endif
