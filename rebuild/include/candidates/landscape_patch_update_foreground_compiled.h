#ifndef FABLETLC_LANDSCAPE_PATCH_UPDATE_FOREGROUND_COMPILED_H
#define FABLETLC_LANDSCAPE_PATCH_UPDATE_FOREGROUND_COMPILED_H

#include <stddef.h>
#include <new>
#include "rebuild_abi.h"

enum LS_LOADING_STATUS
{
    LS_LOADING_READY = 1,
    LS_LOADING_PENDING = 2,
    LS_LOADING_FAILED = 3
};

class CBankFileAsyncEntry {};

class CCountedReleaseTarget {};
typedef void (CCountedReleaseTarget::*CCountedReleaseFunction)();

struct CCountedReference
{
    long Count;
    CCountedReleaseFunction Release;
    void* Owner;
};

template<class T>
class CCountedPointer
{
public:
    T* Pointer;
    CCountedReference* Reference;

    CCountedPointer(T* pointer, CCountedReference* reference)
        : Pointer(pointer), Reference(reference)
    {
        if (Reference != 0)
            ++Reference->Count;
    }

    CCountedPointer(const CCountedPointer& other)
        : Pointer(other.Pointer), Reference(other.Reference)
    {
        if (Reference != 0)
            ++Reference->Count;
    }

    ~CCountedPointer()
    {
        if (Reference != 0 && --Reference->Count == 0)
        {
            (static_cast<CCountedReleaseTarget*>(Reference->Owner)->*Reference->Release)();
            operator delete(Reference);
        }
    }
};

class CEngineLandscapeMap
{
public:
    bool IsBackgroundAtFullDetailInArea(
        unsigned long x,
        unsigned long y,
        unsigned long width,
        unsigned long height);
};

class CWaterPatchMesh
{
public:
    void ClearTextures();
};

class CBankFileAsyncData
{
public:
    fable_u8 Storage[8];
    void DiscardAsyncData();
};

extern fable_u8* DAT_01436ea0;
extern fable_u8* DAT_01436ea8;
extern fable_u8 DAT_013d2880;

class CEngineLandscapePatch
{
public:
    fable_u8 Unknown00[4];
    CEngineLandscapeMap* LandscapeMap;
    void* LayerMeshList;
    fable_u8 Unknown0C[8];
    CWaterPatchMesh* WaterPatch;
    fable_u32 FilePosition;
    long ForegroundDataSize;
    float BoundsMinX;
    float BoundsMinY;
    float BoundsMinZ;
    float BoundsMaxX;
    float BoundsMaxY;
    float BoundsMaxZ;
    fable_u16 PatchX;
    fable_u16 PatchY;
    fable_u8 Flags;
    fable_u8 Unknown3D[3];
    CBankFileAsyncData AsyncData;

    LS_LOADING_STATUS UpdateForeground(CCountedPointer<CBankFileAsyncEntry> bankEntry);
    LS_LOADING_STATUS LoadForegroundPatch(CCountedPointer<CBankFileAsyncEntry> bankEntry);
    bool LoadTextures(float distanceSquared);
    void DeleteLayerMeshes();
};

FABLE_STATIC_ASSERT(sizeof(CCountedPointer<CBankFileAsyncEntry>) == 8);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, LandscapeMap) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, WaterPatch) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, ForegroundDataSize) == 0x1C);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, BoundsMinX) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, BoundsMaxZ) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, PatchX) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, Flags) == 0x3C);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, AsyncData) == 0x40);
FABLE_STATIC_ASSERT(sizeof(CEngineLandscapePatch) == 0x48);

#endif
