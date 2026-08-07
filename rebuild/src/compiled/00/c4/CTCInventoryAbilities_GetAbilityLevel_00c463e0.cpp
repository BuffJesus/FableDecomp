#include "rebuild_abi.h"
// CTCInventoryAbilities::GetAbilityLevel @ 0x00c463e0
// mov eax,[esp+4]; mov ecx,[ecx+8]; lea eax,[eax+eax*2]; mov eax,[ecx+eax*4]; ret 4
// Return the first dword of the 12-byte ability record base[index].
struct CAbilityRec { long level; long b; long c; };
struct CTCInventoryAbilities { char pad00[8]; CAbilityRec *m_abilities; long GetAbilityLevel(long index); };
long CTCInventoryAbilities::GetAbilityLevel(long index)
{
    return m_abilities[index].level;
}