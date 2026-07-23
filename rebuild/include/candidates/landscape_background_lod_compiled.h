#ifndef FABLETLC_LANDSCAPE_BACKGROUND_LOD_COMPILED_H
#define FABLETLC_LANDSCAPE_BACKGROUND_LOD_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

enum LS_LOADING_STATUS
{
    LS_LOADING_READY = 1,
    LS_LOADING_PENDING = 2
};

class CLandscapeGPUUpdateQueue {};

class CEngineMap
{
public:
    fable_u8 pad_00[8];
    void* EdgeHeightFile;
    fable_u8 pad_0c[0x50];
    unsigned short BackgroundWidth;
    unsigned short BackgroundHeight;
};

class CEngineStaticMapFileBlockManager
{
public:
    void AddRequest(long blockId, unsigned long blockSize, bool immediate);
    bool IsBlockLoaded(long blockId);
    unsigned char* GetBlockData(long blockId, unsigned long blockSize);
    void BankFileList_RemoveFinished();
};

class CMemoryDataInputStream
{
public:
    fable_u8 storage[0x18];
    explicit CMemoryDataInputStream(unsigned char* data);
    ~CMemoryDataInputStream();
};

FABLE_STATIC_ASSERT(sizeof(CMemoryDataInputStream) == 0x18);

class CEngineLandscapeMap;
void* __fastcall AllocateLandscapeBackgroundTreeNode(unsigned long bytes, CEngineLandscapeMap* map);

class CLandscapeBackgroundTreeNode
{
public:
    fable_u8 storage[0xE8];
    CLandscapeBackgroundTreeNode(CEngineLandscapeMap* map, void* parent, long mapIndex);
    void BuildDefaultSection(unsigned long x, unsigned long y, unsigned long width, unsigned long height);
    void LoadHeader(CMemoryDataInputStream& stream);
    LS_LOADING_STATUS UpdateStaticLOD(
        double distanceToCamera,
        bool forceUpdate,
        CEngineStaticMapFileBlockManager* manager);
};

class CEngineLandscapeMap
{
public:
    fable_u8 pad_00[4];
    CLandscapeBackgroundTreeNode* BackgroundTree;
    fable_u8 pad_08[0x0C];
    CEngineMap* OwnerMap;
    void* BackgroundLoadEntry;
    fable_u8 pad_1c[4];
    CEngineStaticMapFileBlockManager* StaticMapFileBlockManager;
    fable_u8 pad_24[0x0C];
    long BackgroundHeaderBlockId;
    unsigned long BackgroundHeaderBlockSize;
    fable_u8 pad_38[0x20];
    long LandscapeMapIndex;
    bool LandscapeUpdateActive;
    bool ForegroundLODUpdateActive;
    bool BackgroundHeaderRequestIssued;

    LS_LOADING_STATUS UpdateBackgroundLOD(
        CLandscapeGPUUpdateQueue& gpuUpdateQueue,
        double distanceToCamera,
        bool forceUpdate);
    void ReleaseFileBlockLoad();
};

FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, BackgroundTree) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, OwnerMap) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, BackgroundLoadEntry) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, StaticMapFileBlockManager) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, BackgroundHeaderBlockId) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, LandscapeMapIndex) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, LandscapeUpdateActive) == 0x5C);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, BackgroundHeaderRequestIssued) == 0x5E);

#endif
