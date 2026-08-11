#pragma optimize("g",on)
// CBuyHouseDef::Copy @ 004e01a7
struct CDefClassBase { char pad[0x40]; };

// Base class whose Copy is __fastcall (this=ecx, src on stack, EDX untouched).
struct CBuyHouseBase {
    void BaseCopy(CDefClassBase const* src); // __fastcall member (relocation-masked target)
};

struct CBuyHouseDef {
    char pad[0x25];
    unsigned char field25; // 0x25
    void Copy(CDefClassBase const* src);
};

void __fastcall CBuyHouseDef_Copy(CBuyHouseDef* self, int /*edx*/, CDefClassBase const* src)
{
    ((CBuyHouseBase*)self)->BaseCopy(src);
    self->field25 = ((const CBuyHouseDef*)src)->field25;
}