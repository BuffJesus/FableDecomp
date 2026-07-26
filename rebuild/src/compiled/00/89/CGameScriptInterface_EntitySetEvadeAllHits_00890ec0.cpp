struct Entity {
    char _pad_6c[0x6c];
    unsigned char f6c;   // +0x6c
    char _pad_91[0x24];  // to 0x91
    unsigned char f91;   // +0x91
    char _pad_bc[0x2a];  // to 0xbc
    unsigned char fbc;   // +0xbc
};

// CGameScriptInterface exposes a virtual accessor at vtable slot 11 (+0x2c)
// that returns the current controlled entity.
struct CGameScriptInterface {
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual void s9();
    virtual void s10();
    virtual Entity* GetEntity();   // slot 11 -> [vt+0x2c]
};

void __stdcall CGameScriptInterface_EntitySetEvadeAllHits(CGameScriptInterface* self, char param)
{
    Entity* e = self->GetEntity();
    if (e == 0) return;
    if (e->f91 & 1) return;
    if (!(e->f6c & 2)) return;
    unsigned char v = (unsigned char)param;
    v <<= 3;
    v ^= e->fbc;
    v &= 8;
    e->fbc ^= v;
}