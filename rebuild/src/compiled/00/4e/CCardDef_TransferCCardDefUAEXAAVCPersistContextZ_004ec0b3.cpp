#pragma optimize("s",on)
struct CSubA { char b[4]; };
struct CSubB { char b[4]; };

struct CPersistContext {
    // this=ecx=ctx, member pointer pushed on stack -> __fastcall push;call
    void TransferA(CSubA* m);   // callee at 0x431102
};

struct CPlayerDef {
    char pad3c[0x3c];
    CSubA m3c;   // sub-object at +0x3c
    CSubB m40;   // sub-object at +0x40
    void Transfer(CPersistContext& ctx);
    void TransferB(CPersistContext& ctx); // base transfer at 0x4595a3, takes &self->m40
};

void CPlayerDef::Transfer(CPersistContext& ctx)
{
    ctx.TransferA(&this->m3c);
    // second call: this=ecx=ctx, pushes &self->m40, target 0x4595a3
    reinterpret_cast<CPersistContext*>(&ctx)->TransferA(reinterpret_cast<CSubA*>(&this->m40));
}