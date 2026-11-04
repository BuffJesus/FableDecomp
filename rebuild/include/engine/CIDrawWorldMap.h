#pragma once
#include "fable_string.h"
#include "engine/C2DBoxI.h"
#include "fable_extents_i.h"
class CIDrawMap;
// Donor CDrawMapInfo fields; retail B401E0 uses its 0x20-byte hidden result,
// PMap at4 and the string destructor at8.
class CDrawMapInfo {
public:
    bool Valid,Loadable,GameLoadable;
    CIDrawMap* PMap;
    CCharString StaticMeshName;
    C2DCoordI Pos;
    long UID;
    C2DExtentsI Extents;
};
FABLE_STATIC_ASSERT(sizeof(CDrawMapInfo)==0x20);
FABLE_STATIC_ASSERT(offsetof(CDrawMapInfo,PMap)==4);
FABLE_STATIC_ASSERT(offsetof(CDrawMapInfo,StaticMeshName)==8);
// Partial virtual interface: only the first two donor slots are modelled.
class CIDrawWorldMap {
public:
    virtual long DrawGetNoMaps()=0;
    virtual CDrawMapInfo DrawGetMapInfo(long)=0;
};
