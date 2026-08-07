#include "rebuild_abi.h"
// CGenericVertex<...>::GetU1 @ 0x00c1ba00
// push ecx; movzx eax,byte[ecx+1]; mov [esp],eax; fild [esp]; fmul ds:k; pop ecx; ret
// Convert unsigned 8-bit U coordinate to float via a scale constant.
extern const float kUvScale_c1ba00;
struct CGenericVertex {
    char pad00[1];
    unsigned char m_u1; // +0x1
    float GetU1();
};
float CGenericVertex::GetU1()
{
    return (float)m_u1 * kUvScale_c1ba00;
}