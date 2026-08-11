#include <cstdio>

struct CTCElectrocutedEffect { int tag; };

struct GlobalThing {
    int calls;
    int lastArg;
    void Register(int arg);   // out-of-line => real emitted symbol
};

void GlobalThing::Register(int arg) { calls++; lastArg = arg; }

static GlobalThing g_obj = {0,-1};
GlobalThing* g_electroGlobal = &g_obj;

static int g_baseCalled = 0;
static CTCElectrocutedEffect* g_baseSelf = 0;

void __fastcall CTCElectrocutedEffect_base_OnInitialActivate(CTCElectrocutedEffect* self)
{
    g_baseCalled++;
    g_baseSelf = self;
}

void __fastcall OnInitialActivate(CTCElectrocutedEffect* self);

int main()
{
    CTCElectrocutedEffect e; e.tag = 42;
    OnInitialActivate(&e);
    if (g_obj.calls == 1 && g_obj.lastArg == 0 &&
        g_baseCalled == 1 && g_baseSelf == &e)
        printf("0076c830_TEST PASS\n");
    else
        printf("FAIL c=%d a=%d b=%d\n", g_obj.calls, g_obj.lastArg, g_baseCalled);
    return 0;
}