#include "engine/CWorldMap.h"

struct CWorldMap_Methods : CWorldMap {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8(); virtual void v9();
    virtual bool IsChangeableImpl(int mapIndex);
    bool IsMapChangeable(int mapIndex);
};

bool CWorldMap_Methods::IsMapChangeable(int mapIndex) {
    return IsChangeableImpl(mapIndex);
}
