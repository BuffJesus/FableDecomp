#include "engine/CNavSwitchableLeafNode.h"

struct CNavSwitchableLeafNode_Methods : CNavSwitchableLeafNode {
    long GetRegionID();
};

long CNavSwitchableLeafNode_Methods::GetRegionID()
{
    if (!this->m_disabled) {
        long* linkedRegion = *(long**)&this->_pad_0x10[0];
        if (linkedRegion) {
            return *linkedRegion;
        }
    }
    return 0;
}
