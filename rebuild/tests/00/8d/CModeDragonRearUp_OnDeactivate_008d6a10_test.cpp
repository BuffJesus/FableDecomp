#include <cstdio>

struct CModeDragonRearUp;

struct SomeState {
    char pad[0x10];
    unsigned char flag;   // +0x10
    void Helper(int a, CModeDragonRearUp* self);
};

struct CModeDragonRearUp {
    void* vfptr;
    SomeState* state;
    void OnDeactivate();
};

static int g_helper_called = 0;
static int g_base_called = 0;

void SomeState::Helper(int a, CModeDragonRearUp* self)
{
    (void)a; (void)self;
    g_helper_called++;
}

void __fastcall DragonRearUpBase_861d00(CModeDragonRearUp* self)
{
    (void)self;
    g_base_called++;
}

void CModeDragonRearUp::OnDeactivate()
{
    SomeState* s = this->state;
    if (s->flag == 1)
        s->Helper(0, this);
    DragonRearUpBase_861d00(this);
}

int main()
{
    SomeState s1; s1.flag = 1;
    SomeState s0; s0.flag = 0;

    CModeDragonRearUp a; a.state = &s1;
    CModeDragonRearUp b; b.state = &s0;

    g_helper_called = 0; g_base_called = 0;
    a.OnDeactivate();
    bool ok1 = (g_helper_called == 1 && g_base_called == 1);

    g_helper_called = 0; g_base_called = 0;
    b.OnDeactivate();
    bool ok2 = (g_helper_called == 0 && g_base_called == 1);

    if (ok1 && ok2) { printf("REARUP_DEACT_OK\n"); return 0; }
    printf("FAIL h=%d b=%d\n", g_helper_called, g_base_called);
    return 1;
}