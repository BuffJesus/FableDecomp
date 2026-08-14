// CSoundAtmospheresDef::Copy 0x004e3fd0
// Real C++ member methods -> native __fastcall (this in ecx, src on stack,
// callee cleans with ret 4). Retail never touches edx; member calls avoid it.
struct CDefClassBase;

struct CSubMember {              // member at +0x28, own Copy @ 0x432ee9
    void Copy(const CDefClassBase* src);
};

struct CBaseDef {               // base copy @ 0x431f10, invoked on self+0
    void Copy(const CDefClassBase* src);
};

struct CSoundAtmospheresDef {
    char pad[0x28];
    CSubMember sub;             // at +0x28
    void Copy(const CDefClassBase* src);
};

void CSoundAtmospheresDef::Copy(const CDefClassBase* src)
{
    ((CBaseDef*)this)->Copy(src);
    const CDefClassBase* srcSub = (const CDefClassBase*)((char*)src + 0x28);
    this->sub.Copy(srcSub);
}