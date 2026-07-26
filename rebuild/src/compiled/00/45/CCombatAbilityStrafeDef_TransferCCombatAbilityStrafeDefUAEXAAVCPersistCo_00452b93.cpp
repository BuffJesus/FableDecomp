#pragma optimize("s",on)
struct CFieldA { int a; };
struct CFieldB { int b; };

struct CPersistContext {
    void TransferA(CFieldA* p);
    void TransferB(CFieldB* p);
};

struct CCombatAbilityStrafeDef {
    unsigned char _pad28[0x28];
    CFieldB f28;
    unsigned char _pad40[0x40 - 0x28 - sizeof(CFieldB)];
    CFieldA f40;
};

void __fastcall CCombatAbilityStrafeDef_Transfer(CCombatAbilityStrafeDef* self, int, CPersistContext* ctx)
{
    ctx->TransferA(&self->f40);
    ctx->TransferB(&self->f28);
}