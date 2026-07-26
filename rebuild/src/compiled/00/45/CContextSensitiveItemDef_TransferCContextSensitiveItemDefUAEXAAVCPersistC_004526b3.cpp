#pragma optimize("s",on)
struct Field { unsigned char b[4]; };
struct CPersistContext {
    void PersistA(void* p);
    void PersistB(void* p);
};
struct CContextSensitiveItemDef {
    unsigned char _pad[0x28];
    Field f0; // 0x28
    Field f1; // 0x2c
    Field f2; // 0x30
    Field f3; // 0x34
    Field f4; // 0x38
    void Transfer(CPersistContext& ctx);
};

void CContextSensitiveItemDef::Transfer(CPersistContext& ctx)
{
    ctx.PersistA(&f0);
    ctx.PersistB(&f1);
    ctx.PersistB(&f2);
    ctx.PersistB(&f3);
    ctx.PersistB(&f4);
}