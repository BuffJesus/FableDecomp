#include "rebuild_abi.h"
// CBankFile::IsAligned @ 0x00c4bfa0
// mov eax,[esp+4]; xor edx,edx; div [ecx+0xc]; mov eax,edx; neg;sbb;inc; ret 4
// Return whether n is an exact multiple of this->m_alignment (n % align == 0).
struct CBankFile { char pad00[0xc]; unsigned int m_alignment; bool IsAligned(unsigned int n); };
bool CBankFile::IsAligned(unsigned int n)
{
    return (n % m_alignment) == 0;
}