// CParentDefClassBase::GetInstantiationName  @ 0x004303b0
// __fastcall, returns a CDefString (4-byte wrapper) by value via hidden ptr.
// Loads a DWORD field at this+0x25.
#pragma pack(push,1)
struct CDefString {
    unsigned long value;
    CDefString() {}
    CDefString(const CDefString &o) : value(o.value) {}
};
struct CParentDefClassBase {
    char pad[0x25];
    unsigned long instantiationName; // at +0x25
    CDefString GetInstantiationName();
};
#pragma pack(pop)

CDefString CParentDefClassBase::GetInstantiationName()
{
    CDefString r;
    r.value = this->instantiationName;
    return r;
}