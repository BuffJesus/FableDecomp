#include "engine/CFireballSpellLevelDef.h"

struct CDefClassBase;

struct CFireballSpellLevelDef_Methods : CFireballSpellLevelDef {
    void InheritFrom(CDefClassBase const* src);
    void Copy(CDefClassBase const* src);
};

void CFireballSpellLevelDef_Methods::Copy(CDefClassBase const* src)
{
    this->InheritFrom(src);
    this->SpellLevel = *(unsigned long const*)((char const*)src + 0x28);
}
