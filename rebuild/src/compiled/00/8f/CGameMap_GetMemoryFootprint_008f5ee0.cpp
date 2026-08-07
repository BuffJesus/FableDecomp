#include "rebuild_abi.h"
// CGameMap::GetMemoryFootprint @ 0x008f5ee0
// mov eax,[ecx+0x208]; imul eax,[ecx+0x204]; lea eax,[eax*3]; shl eax,2; ret
// Footprint = height * width * 12 bytes per cell.
struct CGameMap { char pad00[0x204]; long m_width; long m_height; long GetMemoryFootprint(); };
long CGameMap::GetMemoryFootprint()
{
    return (m_height * m_width) * 12;
}