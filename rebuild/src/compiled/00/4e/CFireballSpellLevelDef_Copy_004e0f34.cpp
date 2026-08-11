// CFireballSpellLevelDef::Copy @ 004e0f34
struct CDefClassBase;

struct CFireballSpellLevelDef {
    char pad[0x28];
    unsigned long field28; // offset 0x28
    // Base-class copy, a separate __fastcall function (call rel32 in retail):
    // ecx=this, one stack arg (src), no edx setup.
    void InheritFrom(CDefClassBase const* src);
    void Copy(CDefClassBase const* src);
};

void CFireballSpellLevelDef::Copy(CDefClassBase const* src)
{
    this->InheritFrom(src);
    this->field28 = *(unsigned long const*)((char const*)src + 0x28);
}