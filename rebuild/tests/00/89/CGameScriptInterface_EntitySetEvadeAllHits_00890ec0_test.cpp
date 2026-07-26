#include <cstdio>

struct Entity {
    char _pad_6c[0x6c];
    unsigned char f6c;
    char _pad_91[0x24];
    unsigned char f91;
    char _pad_bc[0x2a];
    unsigned char fbc;
};

struct CGameScriptInterface {
    virtual void s0() = 0;
    virtual void s1() = 0;
    virtual void s2() = 0;
    virtual void s3() = 0;
    virtual void s4() = 0;
    virtual void s5() = 0;
    virtual void s6() = 0;
    virtual void s7() = 0;
    virtual void s8() = 0;
    virtual void s9() = 0;
    virtual void s10() = 0;
    virtual Entity* GetEntity() = 0;
};

static Entity* g_ent;

struct TestGSI : public CGameScriptInterface {
    void s0() {} void s1() {} void s2() {} void s3() {} void s4() {}
    void s5() {} void s6() {} void s7() {} void s8() {} void s9() {} void s10() {}
    Entity* GetEntity() { return g_ent; }
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

int main()
{
    TestGSI self;
    CGameScriptInterface* pself = &self;

    Entity e;
    for (int i = 0; i < (int)sizeof(e); i++) ((char*)&e)[i] = 0;
    g_ent = &e;

    e.f6c = 0x02;
    e.f91 = 0x00;
    e.fbc = 0x00;

    // param bit0 set -> (1<<3)&8 = 8 -> sets bit3 of fbc
    CGameScriptInterface_EntitySetEvadeAllHits(pself, 1);
    if (e.fbc != 0x08) { std::printf("FAIL set bit: %02x\n", e.fbc); return 1; }

    // param bit0 clear -> clears bit3
    CGameScriptInterface_EntitySetEvadeAllHits(pself, 0);
    if (e.fbc != 0x00) { std::printf("FAIL clear bit: %02x\n", e.fbc); return 1; }

    // idempotent set
    CGameScriptInterface_EntitySetEvadeAllHits(pself, 1);
    CGameScriptInterface_EntitySetEvadeAllHits(pself, 1);
    if (e.fbc != 0x08) { std::printf("FAIL idem: %02x\n", e.fbc); return 1; }

    // ineligible: f6c bit1 clear -> no change
    e.fbc = 0x55;
    e.f6c = 0x00;
    CGameScriptInterface_EntitySetEvadeAllHits(pself, 0);
    if (e.fbc != 0x55) { std::printf("FAIL ineligible: %02x\n", e.fbc); return 1; }

    // f91 bit0 set -> skip
    e.f6c = 0x02;
    e.f91 = 0x01;
    e.fbc = 0x00;
    CGameScriptInterface_EntitySetEvadeAllHits(pself, 1);
    if (e.fbc != 0x00) { std::printf("FAIL f91: %02x\n", e.fbc); return 1; }

    // null entity -> no crash
    g_ent = 0;
    CGameScriptInterface_EntitySetEvadeAllHits(pself, 1);

    std::printf("CGameScriptInterface_00890ec0_TEST PASS\n");
    return 0;
}