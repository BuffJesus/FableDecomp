#pragma once
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_carray.h"
#include "fable_stlport_list.h"
#include "engine/CStaticMapBankFile.h"
#include "engine/CEngineMap.h"
class CIDrawWorld;
class CIDrawWorldMap;
class CWideString;
class CEngineMap;
// Partial retail layout: 00B42390/00B423B0 and donor named MapList/VisibleMapList.
// Donor later fields differ substantially; this prefix is not an allocation size.
class CEngineWorldMap {
public:
    void CloseStaticMapFile();
    void SetStaticMapFileForUse(const CWideString&);
    void SetWorld(const CIDrawWorld*);
    void AddMap(long,long);
    void DeleteMap(long);
    void OnGameMapLoaded(long);
    void OnGameMapUnloaded(long);
    void SetMapVisibility(long,bool);
    void SetMapVisibilityAllMaps(bool);
    unsigned char UnknownPrefix[0x0c];
    short* Map;
    long MapArrayWidth,MapArrayHeight,MapOffsetX,MapOffsetY;
    CArrayVector<CEngineMap*> MapList;
    _STL::list<CEngineMap*> VisibleMapList;
    CWideString StaticMapBankName;
    CStaticMapBankFile StaticMapFile;
    const CIDrawWorld* GameWorld;
    CIDrawWorldMap* GameWorldMap;
    long StaticMapQuality;
    bool EnableMinimumStaticMapRebuild,FirstWorldUpdate;
    long StaticMapOpenState;
    bool DynamicUpdateEnabled,BuildRetailStaticMaps;
    long StaticMapBuildStatus;
private:
    void OpenStaticMaps(CEngineMap::STATIC_MAP_OPEN_STATE);
    void OpenStaticMapsUsingCommonRetailHeader(long);
    void LoadWaterData();
    void DeleteMap();
    void AutoBuildWorldMapGrid();
    void LinkMapToWorld(CEngineMap*,long);
    void UnlinkMapFromWorld(CEngineMap*,long);
};
FABLE_STATIC_ASSERT(offsetof(CEngineWorldMap,MapList)==0x20);
FABLE_STATIC_ASSERT(offsetof(CEngineWorldMap,VisibleMapList)==0x2c);
FABLE_STATIC_ASSERT(offsetof(CEngineWorldMap,GameWorldMap)==0x19c);
FABLE_STATIC_ASSERT(offsetof(CEngineWorldMap,StaticMapFile)==0x34);
FABLE_STATIC_ASSERT(offsetof(CEngineWorldMap,StaticMapOpenState)==0x1a8);
FABLE_STATIC_ASSERT(offsetof(CEngineWorldMap,StaticMapBuildStatus)==0x1b0);
