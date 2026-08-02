#include <cstdio>

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

static int g_called = 0;
static CCombatActionBase* g_self = 0;

extern "C" void __fastcall FrameUpdate_CCombatActionBase(CCombatActionBase* self)
{
    g_called = 1;
    g_self = self;
}

int main()
{
    CCreatureAction_StartBlocking obj;
    obj.field_10 = 0;

    obj.FrameUpdate();

    if (obj.field_10 == 2 && g_called == 1 && g_self == (CCombatActionBase*)&obj)
    {
        printf("TESTPASSMARKER_008552e0\n");
        return 0;
    }

    printf("FAIL field_10=%d called=%d\n", obj.field_10, g_called);
    return 1;
}