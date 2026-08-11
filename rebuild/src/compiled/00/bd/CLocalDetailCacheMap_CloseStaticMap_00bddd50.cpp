// CLocalDetailCacheMap::CloseStaticMap @ 0x00bddd50

// Subobject at this+0xc; helper @0xbddc00 is its thiscall method:
// ecx = &sub, single stack arg = this->f4.
struct SubObj {
    void Method(int arg);   // __fastcall by default (member)
};

struct CLocalDetailCacheMap {
    int    f0;      // +0x00
    int    f4;      // +0x04
    int    f8;      // +0x08
    SubObj sub;     // +0x0c
};

void __fastcall CloseStaticMap(CLocalDetailCacheMap* self)
{
    self->sub.Method(self->f4);
    self->f4 = 0;
}