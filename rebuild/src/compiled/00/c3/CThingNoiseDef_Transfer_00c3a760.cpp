struct CPersistContext;

struct CThingNoiseDef_Base
{
    char base_bytes[0x18];
    void __fastcall Transfer(CPersistContext* ctx);
};

struct CThingNoiseMember
{
    void DoIt(void* owner, int flag);
};

struct CThingNoiseDef : public CThingNoiseDef_Base
{
    CThingNoiseMember m18;
    CThingNoiseDef* __fastcall Transfer(CPersistContext* ctx);
};

CThingNoiseDef* __fastcall CThingNoiseDef::Transfer(CPersistContext* ctx)
{
    this->CThingNoiseDef_Base::Transfer(ctx);
    this->m18.DoIt(this, 1);
    return this;
}