#pragma once
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_stlport_list.h"
#include "fable_stlport_tree.h"
#include "fable_counted_pointer.h"
#include "fable_string.h"
#include "fable_2d_vector.h"
#include "fable_bounding_volumes.h"
#include "engine/CEngineStaticMapFileBlockManager.h"
class CEngineLandscapeMap;
class CLocalDetailCacheMap;
class CIDrawMap;
class CBankFileAsyncEntry;
class CStaticMapBankFile;
class CDataInputStream;
// Partial retail prefix; visibility offsets verified at 00B41DB0.
class CEngineMap {
public:
    enum STATIC_MAP_OPEN_STATE { STATIC_MAP_OPEN_STATE_CLOSED=0, STATIC_MAP_OPEN_STATE_OPEN_FOR_USE=1, STATIC_MAP_OPEN_STATE_OPEN_FOR_UPDATE=2 };
    CEngineMap(const CIDrawMap*,long x,long y,long width,long height,long uid,CCharString name,bool);
    ~CEngineMap();
    void OnGameMapUnloaded();
    void CloseStaticMap();
    void OpenDevelopmentStaticMap(CStaticMapBankFile&,STATIC_MAP_OPEN_STATE);
    void OpenRetailStaticMap(CStaticMapBankFile&,CDataInputStream&,_STL::map<CCharString,long>&);
    struct CStaticMapInfoBlock {
        long VersionID,BankFileIndex,EdgeHeightFilePtr,EdgeHeightFileSize;
        long LandscapeMapPtr,LocalDetailMapPtr,ChecksumBlockFilePtr,ChecksumBlockFileSize;
        long MapWidth,MapHeight,WorldX,WorldY,Quality,ShorePointArraySize,ShorePointArrayStart,LevelChecksum;
        C3DBoundingBox CameraMapBounds;
        long HeaderEndPtr;
    };
    void SetMapVisibility(_STL::list<CEngineMap*>&,bool);
    CEngineLandscapeMap* LandscapeMap;
    CLocalDetailCacheMap* LocalDetailCacheMap;
    union { const CIDrawMap* GameMap; void* EdgeHeightFile; }; // Preserve legacy LOD field alias.
    C3DVector MapWorldPosition;
    unsigned long MapUID;
    CCharString StaticMapName;
    long StaticMapOpenState;
    C3DBoundingBox CameraMapBounds;
    CCountedPointer<CBankFileAsyncEntry> AsyncFileEntry;
    CEngineStaticMapFileBlockManager FileBlockManager;
    _STL::list<CEngineMap*>::iterator VisibleMapListIterator;
    union { unsigned short MapWidth; unsigned short BackgroundWidth; };
    union { unsigned short MapHeight; unsigned short BackgroundHeight; };
    unsigned short WorldPosX,WorldPosY;
    bool IsInVisibleMapList;
private:
    static void FABLE_FASTCALL ReadMapInfoBlock(CStaticMapInfoBlock&,CDataInputStream&);
};
FABLE_STATIC_ASSERT(sizeof(CEngineMap::CStaticMapInfoBlock)==0x5c);
FABLE_STATIC_ASSERT(offsetof(CEngineMap,EdgeHeightFile)==8);
FABLE_STATIC_ASSERT(offsetof(CEngineMap,StaticMapName)==0x1c);
FABLE_STATIC_ASSERT(offsetof(CEngineMap,FileBlockManager)==0x44);
FABLE_STATIC_ASSERT(offsetof(CEngineMap,StaticMapOpenState)==0x20);
FABLE_STATIC_ASSERT(offsetof(CEngineMap,AsyncFileEntry)==0x3c);
FABLE_STATIC_ASSERT(offsetof(CEngineMap,BackgroundWidth)==0x5c);
FABLE_STATIC_ASSERT(offsetof(CEngineMap,VisibleMapListIterator)==0x58);
FABLE_STATIC_ASSERT(offsetof(CEngineMap,IsInVisibleMapList)==0x64);
