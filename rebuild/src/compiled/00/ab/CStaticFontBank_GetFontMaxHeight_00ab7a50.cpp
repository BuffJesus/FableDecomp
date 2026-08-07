#include "rebuild_abi.h"
// CStaticFontBank::GetFontMaxHeight @ 0x00ab7a50
// push ecx; mov eax,[ecx+4]; fild [ecx+4]; test eax,eax; jge L; fadd ds:2^32; L: pop ecx; ret
// Convert the unsigned 32-bit height field to float: (float)(unsigned)this->f4.
struct CStaticFontBank {
    char pad00[4];
    unsigned int m_maxHeight; // +0x4
    float GetFontMaxHeight();
};
float CStaticFontBank::GetFontMaxHeight()
{
    return (float)m_maxHeight;
}