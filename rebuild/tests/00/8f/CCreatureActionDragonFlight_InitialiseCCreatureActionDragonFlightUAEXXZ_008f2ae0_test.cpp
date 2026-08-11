#include <stdio.h>

// ---- standalone copies of the extern deps for the test link ----
float g_dragonFlightScale = 2.5f;

struct CCreatureAction_Base
{
    virtual void Initialise();
};

struct CCreatureAction_DragonFlight : public CCreatureAction_Base
{
    char pad4[0x10 - 4];
    int  field10;
    int  field14;
    virtual void Initialise();
};

static int g_baseCalled = 0;
void __fastcall CCreatureAction_Base_Initialise(CCreatureAction_Base* self)
{
    (void)self;
    g_baseCalled = 1;
}

void CCreatureAction_Base::Initialise() {}

void CCreatureAction_DragonFlight::Initialise()
{
    CCreatureAction_Base_Initialise(this);
    this->field14 = (int)((float)this->field10 * g_dragonFlightScale);
}

int main()
{
    CCreatureAction_DragonFlight a;
    a.field10 = 10;
    a.field14 = -1;
    g_baseCalled = 0;

    a.Initialise();

    // 10 * 2.5 = 25.0 -> (int)25
    int ok = (g_baseCalled == 1) && (a.field14 == 25);

    // negative truncation check: -3 * 2.5 = -7.5 -> ftol truncates toward zero -> -7
    a.field10 = -3;
    a.Initialise();
    ok = ok && (a.field14 == -7);

    printf(ok ? "DRAGONFLIGHT_INIT_OK\n" : "FAIL\n");
    return ok ? 0 : 1;
}