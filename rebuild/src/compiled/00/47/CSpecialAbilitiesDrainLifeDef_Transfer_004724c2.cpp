#pragma optimize("s",on)
struct Field { unsigned char b[0xc]; };
struct CPersistContext {
    void PersistA(void* p);
    void PersistB(void* p);
    void PersistC(void* p);
    void PersistD(void* p);
};
struct CSpecialAbilitiesDrainLifeDef {
    unsigned char _pad[0x3c];
    Field f0; // 0x3c
    Field f1; // 0x48
    Field f2; // 0x54
    Field f3; // 0x60
    Field f4; // 0x6c
    void Transfer(CPersistContext& ctx);
};

void CSpecialAbilitiesDrainLifeDef::Transfer(CPersistContext& ctx)
{
    ctx.PersistA(&f0);
    ctx.PersistB(&f1);
    ctx.PersistA(&f2);
    ctx.PersistC(&f3);
    ctx.PersistD(&f4);
}