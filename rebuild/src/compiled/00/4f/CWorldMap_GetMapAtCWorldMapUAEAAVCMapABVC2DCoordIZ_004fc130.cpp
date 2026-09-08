#include "engine/CWorldMap.h"

class CEditMap;
class C2DCoordI;
struct RetailMapEntry {
    unsigned char _pad_0x00[0x10];
    CEditMap* map;
    unsigned char _pad_0x14[0x34];
};
struct CWorldMap_Methods : CWorldMap {
    virtual int vf0(); virtual int vf1(); virtual int vf2(); virtual int vf3();
    virtual int vf4(); virtual int vf5(); virtual int vf6(); virtual int vf7();
    virtual int vf8(); virtual int vf9(); virtual int vf10(); virtual int vf11();
    virtual int vf12(); virtual int vf13(); virtual int vf14(); virtual int vf15();
    virtual int vf16(); virtual int GetIndex(const C2DCoordI* coordinate);
    CEditMap* GetMapAt(const C2DCoordI* coordinate);
};

CEditMap* CWorldMap_Methods::GetMapAt(const C2DCoordI* coordinate) {
    int index = GetIndex(coordinate);
    CWorldMap* worldMap = reinterpret_cast<CWorldMap*>(this);
    RetailMapEntry* entries = *reinterpret_cast<RetailMapEntry**>(
        worldMap->LoadedScriptedMapBrushes + 0x0c);
    return entries[index].map;
}
