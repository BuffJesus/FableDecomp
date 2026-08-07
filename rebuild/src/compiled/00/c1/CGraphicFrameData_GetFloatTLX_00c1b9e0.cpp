#include "rebuild_abi.h"
// CGraphicFrameData::GetFloatTLX @ 0x00c1b9e0
// push ecx; movzx eax,word[ecx+8]; mov [esp],eax; fild [esp]; fmul ds:k; pop ecx; ret
// Convert unsigned 16-bit texel coord to normalised float via a scale constant.
extern const float kTexScale_c1b9e0;
struct CGraphicFrameData {
    char pad00[8];
    unsigned short m_tlx; // +0x8
    float GetFloatTLX();
};
float CGraphicFrameData::GetFloatTLX()
{
    return (float)m_tlx * kTexScale_c1b9e0;
}