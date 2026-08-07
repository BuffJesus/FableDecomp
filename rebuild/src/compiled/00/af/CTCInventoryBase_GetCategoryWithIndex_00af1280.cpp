#include "rebuild_abi.h"
// CTCInventoryBase::GetCategoryWithIndex @ 0x00af1280
// mov eax,[esp+4]; mov ecx,[ecx+0x1e0]; lea eax,[eax+eax*2]; lea eax,[ecx+eax*4]; ret 4
// Return &base[index], where each CCategory is 12 bytes and base = this->m_categories.
struct CCategory { char data[12]; };
struct CTCInventoryBase {
    char pad00[0x1e0];
    CCategory *m_categories; // +0x1e0
    CCategory *GetCategoryWithIndex(long index);
};
CCategory *CTCInventoryBase::GetCategoryWithIndex(long index)
{
    return &m_categories[index];
}