#include <cstdio>

struct Trig { int calls; void* lastArg; };

struct Receptor {
    char pad24[0x24];
    void* field24;
    void* p28;
};

static Trig g_trig;

static bool AddTrigger(Receptor* self, Trig* trig)
{
    trig->calls++;
    trig->lastArg = &self->field24;
    return self->p28 != 0;
}

int main()
{
    Receptor r; g_trig.calls = 0; g_trig.lastArg = 0;
    r.field24 = 0; r.p28 = 0;
    bool a = AddTrigger(&r, &g_trig);
    bool ok1 = (a == false) && (g_trig.calls == 1) && (g_trig.lastArg == &r.field24);

    r.p28 = (void*)1;
    bool b = AddTrigger(&r, &g_trig);
    bool ok2 = (b == true) && (g_trig.calls == 2);

    if (ok1 && ok2) printf("PARITY_OK\n");
    else printf("FAIL\n");
    return 0;
}