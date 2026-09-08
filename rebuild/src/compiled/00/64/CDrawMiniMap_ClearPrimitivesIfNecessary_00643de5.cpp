#pragma optimize("s",on)
#include "engine/CDrawMiniMap.h"

struct MiniMapPrimitiveStorage { void Clear(); };
struct CDrawMiniMap_Methods : CDrawMiniMap {
    virtual void vslot0();
    virtual void ClearDeferredPrimitives();
    void ClearPrimitivesIfNecessary();
};

void CDrawMiniMap_Methods::ClearPrimitivesIfNecessary() {
    CDrawMiniMap* miniMap = reinterpret_cast<CDrawMiniMap*>(this);
    if (miniMap->f_18 == 0)
        reinterpret_cast<MiniMapPrimitiveStorage*>(&miniMap->sub)->Clear();
    if (miniMap->MiniMap[0xbf] == 0)
        ClearDeferredPrimitives();
}
