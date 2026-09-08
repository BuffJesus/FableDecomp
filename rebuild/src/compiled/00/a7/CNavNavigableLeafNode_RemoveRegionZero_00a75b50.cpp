#include "engine/CNavNavigableLeafNode.h"

struct CNavNavigableLeafNode_Methods : CNavNavigableLeafNode {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
    virtual int RemoveRegion();
    bool RemoveRegionZero();
};
bool CNavNavigableLeafNode_Methods::RemoveRegionZero() { return RemoveRegion() == 0; }
