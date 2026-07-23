#ifndef FABLETLC_LANDSCAPE_MAP_UPDATE_FOREGROUND_COMPILED_H
#define FABLETLC_LANDSCAPE_MAP_UPDATE_FOREGROUND_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

enum LS_LOADING_STATUS
{
    LS_LOADING_READY = 1,
    LS_LOADING_PENDING = 2,
    LS_LOADING_FAILED = 3
};

class CMapCountedReleaseTarget {};
typedef void (CMapCountedReleaseTarget::*CMapCountedReleaseFunction)();

struct CMapCountedReference
{
    long Count;
    CMapCountedReleaseFunction Release;
    void* Owner;
};

template<class T>
class CMapCountedPointer
{
public:
    T* Pointer;
    CMapCountedReference* Reference;

    CMapCountedPointer() : Pointer(0), Reference(0) {}

    CMapCountedPointer(T* pointer, CMapCountedReference* reference)
        : Pointer(pointer), Reference(reference)
    {
        if (Reference != 0)
            ++Reference->Count;
    }

    CMapCountedPointer(const CMapCountedPointer& other)
        : Pointer(other.Pointer), Reference(other.Reference)
    {
        if (Reference != 0)
            ++Reference->Count;
    }

    ~CMapCountedPointer() { Clear(); }

    CMapCountedPointer& operator=(const CMapCountedPointer& other)
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
            (static_cast<CMapCountedReleaseTarget*>(Reference->Owner)->*Reference->Release)();
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
    void* GetData();
    unsigned long GetDataSize();
    void DiscardAsyncData();
};

class CBankFileAsyncEntry
{
public:
    CMapCountedPointer<CBankFileAsyncData> BeginReadDataAutoAlloc(
        unsigned long offset,
        unsigned long size);
};

class CMemoryDataInputStream
{
public:
    fable_u8 Storage[0x1C];

    CMemoryDataInputStream(void* data, unsigned long size);
    ~CMemoryDataInputStream();
};

class CEngineLandscapeMap;

class CEngineLandscapePatch
{
public:
    fable_u8 Storage[0x48];

    CEngineLandscapePatch();
    ~CEngineLandscapePatch();
    void Initialise(CEngineLandscapeMap* map, int x, int y);
    void LoadHeader(CMemoryDataInputStream& input);
    LS_LOADING_STATUS UpdateForeground(CMapCountedPointer<CBankFileAsyncEntry> bankEntry);
};

struct CBackgroundTreeBounds
{
    fable_u8 Unknown00[0xA8];
    float MinX;
    float MinY;
    float MinZ;
    float MaxX;
    float MaxY;
    float MaxZ;
};

extern fable_u8* DAT_01436ea0;
extern fable_u8* DAT_01436ea8;
extern fable_u8 DAT_013d2880;

class CEngineLandscapeMap
{
public:
    fable_u8 Unknown00[4];
    CBackgroundTreeBounds* BackgroundTree;
    CEngineLandscapePatch* ForegroundPatches;
    int PatchColumns;
    int PatchRows;
    fable_u8 Unknown14[4];
    CBankFileAsyncEntry* BankEntry;
    CMapCountedReference* BankEntryReference;
    fable_u8 Unknown20[0x0C];
    unsigned long ForegroundBlockOffset;
    fable_u8 Unknown30[8];
    CMapCountedPointer<CBankFileAsyncData> ForegroundAsync;
    fable_u8 Unknown40[0x1C];
    bool UpdateForegroundActive;
    bool KeepForegroundLoaded;

    LS_LOADING_STATUS UpdateForeground();
};

FABLE_STATIC_ASSERT(sizeof(CMapCountedPointer<CBankFileAsyncData>) == 8);
FABLE_STATIC_ASSERT(sizeof(CEngineLandscapePatch) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CBackgroundTreeBounds, MinX) == 0xA8);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, BackgroundTree) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, ForegroundPatches) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, PatchColumns) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, PatchRows) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, BankEntry) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, BankEntryReference) == 0x1C);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, ForegroundBlockOffset) == 0x2C);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, ForegroundAsync) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, UpdateForegroundActive) == 0x5C);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, KeepForegroundLoaded) == 0x5D);
FABLE_STATIC_ASSERT(sizeof(CEngineLandscapeMap) == 0x60);

#endif
