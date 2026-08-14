#include <cstdio>

struct IThing {
    virtual void slot0(int a) = 0;
};

struct CGuiWindow {
    char pad[0xe4];
    IThing* thing;
};

static int g_calls = 0;
static int g_arg = -1;

struct Impl : IThing {
    virtual void slot0(int a) { g_calls++; g_arg = a; }
};

void __fastcall OnResize(CGuiWindow* self)
{
    IThing* t = self->thing;
    if (t != 0) {
        t->slot0(1);
        self->thing = 0;
    }
}

int main()
{
    Impl impl;
    CGuiWindow w;
    w.thing = &impl;
    OnResize(&w);
    if (g_calls != 1) { printf("FAIL calls\n"); return 1; }
    if (g_arg != 1) { printf("FAIL arg\n"); return 1; }
    if (w.thing != 0) { printf("FAIL notcleared\n"); return 1; }

    CGuiWindow w2;
    w2.thing = 0;
    OnResize(&w2);
    if (g_calls != 1) { printf("FAIL extracall\n"); return 1; }
    if (w2.thing != 0) { printf("FAIL nullchanged\n"); return 1; }

    printf("BEHAVIOR_OK\n");
    return 0;
}