#include "rebuild_abi.h"
// CNavSwitchableLeafNode::GetRegionID @ 0x00a75be0
// mov al,[ecx+0xf]; test al; jne L0; mov eax,[ecx+0x10]; test; je L0; mov eax,[eax]; ret; L0: xor eax,eax; ret
// Return the region id from the linked node, or 0 when switched off or unlinked.
struct CNavSwitchableLeafNode {
    char pad00[0xf];
    unsigned char m_disabled;  // +0xf
    long *m_link;              // +0x10
    long GetRegionID();
};
long CNavSwitchableLeafNode::GetRegionID()
{
    if (!m_disabled)
    {
        if (m_link) return *m_link;
    }
    return 0;
}