#pragma optimize("s",on)
// CGiftDef::Copy @ 0x004e077d
struct CDefClassBase;

struct CDefClassBase {
    // base copy is a real thiscall member: this in ecx, src on stack
    void Copy(const CDefClassBase* src);
    char basepad[0x28];
};

struct CGiftDef : CDefClassBase {
    unsigned int f28;   // +0x28
    unsigned char f2c;  // +0x2c
    void Copy(const CDefClassBase* src);
};

void CGiftDef::Copy(const CDefClassBase* src)
{
    CDefClassBase::Copy(src);
    const CGiftDef* s = (const CGiftDef*)src;
    this->f28 = s->f28;
    this->f2c = s->f2c;
}