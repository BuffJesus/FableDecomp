// CThingNoiseDef::Copy @ 0x00463923
struct CDefClassBase;

struct SubMember {
    char pad[4];
    void Copy(const CDefClassBase* src); // @ 0x4537e9  (thiscall, 1 arg)
};

struct CBaseNoiseDef {
    char pad[0x70];
    SubMember member; // at +0x70
    void Copy(const CDefClassBase* src); // @ 0x463950  (thiscall, 1 arg)
};

struct CThingNoiseDef : public CBaseNoiseDef {
    void Copy(const CDefClassBase* src);
};

void CThingNoiseDef::Copy(const CDefClassBase* src)
{
    CBaseNoiseDef::Copy(src);
    this->member.Copy((const CDefClassBase*)((const char*)src + 0x70));
}