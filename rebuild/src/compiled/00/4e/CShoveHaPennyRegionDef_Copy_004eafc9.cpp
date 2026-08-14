#pragma optimize("s",on)
// CShoveHaPennyRegionDef::Copy @ 0x004eafc9
struct CDefClassBase;

struct CShoveHaPennyRegionDef {
    char pad[0x28];
    unsigned int f28;   // +0x28
    unsigned int f2c;   // +0x2c
    void Copy(const CDefClassBase* src);
};

// base Copy at 0x431f10 -- this in ecx, src pushed (__fastcall). Model as
// __fastcall free fn where ONLY ecx is the this-pointer and src is on the stack.
// Use a real member call to force __fastcall codegen.
struct CBaseDef {
    void Copy(const CDefClassBase* src);
};

void CShoveHaPennyRegionDef::Copy(const CDefClassBase* src)
{
    ((CBaseDef*)this)->Copy(src);
    this->f28 = ((const CShoveHaPennyRegionDef*)src)->f28;
    this->f2c = ((const CShoveHaPennyRegionDef*)src)->f2c;
}