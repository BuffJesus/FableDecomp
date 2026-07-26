#pragma optimize("s",on)
struct CPersistContext {
    void TByte(unsigned char* p);
    void TWord(unsigned short* p);
    void TSub(void* p);
};

struct CCombatAbilityAttackBase {
    char pad0[0x28];
    char sub28[0x18];   // +0x28
    unsigned char pad40; // +0x40
    unsigned char pad41; // +0x41
    char pad42[2];
    unsigned short field44; // +0x44
    void Transfer(CPersistContext& ctx);
};

void CCombatAbilityAttackBase::Transfer(CPersistContext& ctx)
{
    ctx.TByte(&this->pad40);
    ctx.TByte(&this->pad41);
    ctx.TWord(&this->field44);
    ctx.TSub(&this->sub28);
}