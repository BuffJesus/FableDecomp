#include <cstdio>

// Behaviour model of InitialiseActionPause

struct Reg { int lastId; void* lastDesc; int calls; };
static Reg g_reg = {0,0,0};

struct Obj {
    int actionId;
    int getId() { return actionId; }
};

static Reg* getRegistrar() { return &g_reg; }
static void registerAction(Reg* r, int id, void* desc) {
    r->lastId = id; r->lastDesc = desc; r->calls++;
}

static void init(Obj* self) {
    int esi = self->getId();
    if (esi > 0) {
        Reg* r = getRegistrar();
        registerAction(r, esi, (void*)0x13bac90);
    }
}

int main() {
    Obj a; a.actionId = 5;
    init(&a);
    bool ok = (g_reg.calls == 1 && g_reg.lastId == 5 && g_reg.lastDesc == (void*)0x13bac90);

    // non-positive => no registration
    g_reg.calls = 0;
    Obj b; b.actionId = 0;
    init(&b);
    ok = ok && (g_reg.calls == 0);

    Obj c; c.actionId = -3;
    init(&c);
    ok = ok && (g_reg.calls == 0);

    if (ok) printf("PARITY_OK\n");
    else printf("FAIL\n");
    return 0;
}