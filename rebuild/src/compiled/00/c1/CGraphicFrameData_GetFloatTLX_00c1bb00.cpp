#include "rebuild_abi.h"
// CGraphicFrameData::GetFloatTLX @ 0x00c1bb00
// push ecx; movzx eax,word[ecx+0xe]; mov [esp],eax; fild [esp]; fmul ds:k; pop ecx; ret
// Convert unsigned 16-bit coord at +0xe to float via a scale constant.
extern const float kTexScale_c1bb00;
struct CGraphicFrameData {
    char pad00[0xe];
    unsigned short m_coord; // +0xe
    float GetFloatTLX();
};
float CGraphicFrameData::GetFloatTLX()
{
    return (float)m_coord * kTexScale_c1bb00;
}