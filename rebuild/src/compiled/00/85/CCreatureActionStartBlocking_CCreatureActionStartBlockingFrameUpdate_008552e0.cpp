struct CCombatActionBase
{
    int vtbl;
    int field_4;
    int field_8;
    int field_c;
    int field_10;
};

extern "C" void __fastcall FrameUpdate_CCombatActionBase(CCombatActionBase* self);

struct CCreatureAction_StartBlocking : CCombatActionBase
{
    void __fastcall FrameUpdate();
};

void __fastcall CCreatureAction_StartBlocking::FrameUpdate()
{
    field_10 = 2;
    FrameUpdate_CCombatActionBase(this);
}