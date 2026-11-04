#pragma once
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CLandscapeBackgroundTreeNode.h"
#include "engine/CEngineLandscapePatch.h"
#include "engine/CEngineTexturePalette.h"
#include "fable_movable_resource.h"
#include "engine/CEngineMap.h"
#include "engine/CEngineStaticMapFileBlockManager.h"
class CLandscapeGPUUpdateQueue;
class CDataInputStream;
class CEngineLandscapeMap;
void* __fastcall AllocateLandscapeBackgroundTreeNode(unsigned long bytes, CEngineLandscapeMap* map);

class CEngineLandscapeMap : public CMovableResource
{
public:
    CEngineLandscapeMap(const CEngineMap*,long);
    virtual ~CEngineLandscapeMap();
    void OnGameMapLoaded();
    void OnGameMapUnloaded();
    void OnMapVisibilityChanged(bool);
    void CloseStaticMap();
    void OpenStaticMap(CEngineStaticMapFileBlockManager*,CDataInputStream&);
    void Clear();
    CLandscapeBackgroundTreeNode* BackgroundTree;
    CEngineLandscapePatch* Patches;
    long PatchGridWidth,PatchGridHeight;
    CEngineMap* OwnerMap;
    CCountedPointer<CBankFileAsyncEntry> MapFileEntry;
    void* GetBackgroundLoadEntry() const { return MapFileEntry.Data; }
    void SetBackgroundLoadEntry(void* entry) { MapFileEntry.Data=static_cast<CBankFileAsyncEntry*>(entry); }
    __declspec(property(get=GetBackgroundLoadEntry,put=SetBackgroundLoadEntry)) void* BackgroundLoadEntry;
    CEngineStaticMapFileBlockManager* StaticMapFileBlockManager;
    struct CMapFileHeader {
        long TexturePaletteFilePointer,TexturePaletteSize,PatchesFilePointer;
        long PatchTreeFileBlockPos,PatchTreeFileBlockSize;
    };
    union {
        CMapFileHeader MapFileHeader;
        struct {
            fable_u8 pad_24[8];
            unsigned long PatchesFilePointer;
            long BackgroundHeaderBlockId;
            unsigned long BackgroundHeaderBlockSize;
        };
    };
    CCountedPointer<CBankFileAsyncData> LoadingPatches;
    CEngineTexturePalette TexturePalette;
    union { long CreationFlags; long LandscapeMapIndex; }; // Preserve legacy LOD alias.
    union { bool LandscapeUpdateActive; bool MemoryFreeingLocked; };
    union { bool ForegroundLODUpdateActive; bool IsDynamic; };
    union { bool BackgroundHeaderRequestIssued; bool PatchTreeBaseLoading; };

    LS_LOADING_STATUS UpdateBackgroundLOD(
        CLandscapeGPUUpdateQueue& gpuUpdateQueue,
        double distanceToCamera,
        bool forceUpdate);
private:
    virtual void RelocateData(long,void*,void*,long);
    virtual bool CanRelocateData(long) const;
    void ReleaseLoadingPatchTreeBase();
};

FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, BackgroundTree) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, Patches) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, OwnerMap) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, MapFileEntry) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, PatchesFilePointer) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, StaticMapFileBlockManager) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, BackgroundHeaderBlockId) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, LoadingPatches) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, LandscapeMapIndex) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, LandscapeUpdateActive) == 0x5C);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, IsDynamic) == 0x5D);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, BackgroundHeaderRequestIssued) == 0x5E);

