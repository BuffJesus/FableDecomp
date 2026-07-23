#ifndef FABLETLC_BACKGROUND_TREE_STATIC_LOD_COMPILED_H
#define FABLETLC_BACKGROUND_TREE_STATIC_LOD_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

enum LS_LOADING_STATUS
{
    LS_LOADING_READY = 1,
    LS_LOADING_PENDING = 2,
    LS_LOADING_FAILED = 3
};

class CEngineStaticMapFileBlockManager {};

struct CStaticLODEntry
{
    fable_u32 Unknown00;
    fable_u32 Unknown04;
    fable_u8 PatchDetailLevel;
    fable_u8 Unknown09[7];
};

class CLoadedStaticLODBase
{
public:
    virtual void Slot00();
    virtual void Slot04();
    virtual void Slot08();
    virtual void Slot0C();
    virtual bool IsPatchObject();
};

class CLoadedStaticLODPatch : public CLoadedStaticLODBase
{
public:
    fable_u8 Unknown04[0x114];
    fable_u8 DetailLevel;
    virtual bool IsPatchObject();
};

class CLandscapeBackgroundTreeNode
{
public:
    fable_u8 Unknown00[0x10];
    CStaticLODEntry StaticLODEntries[8];
    fable_u8 Unknown90[8];
    void* PendingLoadTask;
    fable_u8 Unknown9C[4];
    CLoadedStaticLODBase* LoadedStaticLOD;
    fable_u8 UnknownA4[4];
    float BoundsMinX;
    float BoundsMinY;
    float BoundsMinZ;
    float BoundsMaxX;
    float BoundsMaxY;
    float BoundsMaxZ;
    fable_u32 LoadStateC0;
    fable_u32 LoadStateC4;
    fable_u32 LoadStateC8;
    fable_u32 LoadStateCC;
    fable_u8 UnknownD0[0x10];
    fable_u8 LODStateE0;
    fable_u8 CurrentLODLevel;
    fable_u8 RequestedLODEntryIndex;
    fable_u8 LODStateE3;
    fable_u8 UnknownE4[2];
    fable_u8 HasStaticLODData;
    fable_u8 UnknownE7;

    LS_LOADING_STATUS UpdateStaticLOD(
        double distanceToCamera,
        bool forceUpdate,
        CEngineStaticMapFileBlockManager* manager);
    bool DebugCalcInForgroundRange();
    LS_LOADING_STATUS ProcessLoadTask(
        int foregroundRange,
        double distanceToCamera,
        bool forceUpdate,
        CEngineStaticMapFileBlockManager* manager);
    bool StartPatchLoadTask(
        unsigned long detailLevel,
        CEngineStaticMapFileBlockManager* manager,
        bool immediate);
    bool StartTreeLoadTask(CEngineStaticMapFileBlockManager* manager, bool immediate);
};

class CLoadedStaticLODTree : public CLoadedStaticLODBase
{
public:
    CLandscapeBackgroundTreeNode FirstChild;
    CLandscapeBackgroundTreeNode SecondChild;
    virtual bool IsPatchObject();
};

double GFGetFastSubFrameTimer();
extern fable_u8* DAT_01436ea0;
extern float DAT_0122dedc;

FABLE_STATIC_ASSERT(sizeof(CStaticLODEntry) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CStaticLODEntry, PatchDetailLevel) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CLoadedStaticLODPatch, DetailLevel) == 0x118);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundTreeNode, StaticLODEntries) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundTreeNode, PendingLoadTask) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundTreeNode, LoadedStaticLOD) == 0xA0);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundTreeNode, BoundsMinX) == 0xA8);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundTreeNode, LoadStateC0) == 0xC0);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundTreeNode, CurrentLODLevel) == 0xE1);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundTreeNode, RequestedLODEntryIndex) == 0xE2);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundTreeNode, HasStaticLODData) == 0xE6);
FABLE_STATIC_ASSERT(sizeof(CLandscapeBackgroundTreeNode) == 0xE8);
FABLE_STATIC_ASSERT(offsetof(CLoadedStaticLODTree, FirstChild) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CLoadedStaticLODTree, SecondChild) == 0xEC);

#endif
