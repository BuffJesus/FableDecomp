#include <cstdio>

// Behaviour model: AddTrigger forwards to a global receiver's method with
// (trigger, &self->sub18, self->field20, trigger).

struct Trigger { int id; };

struct Receptor {
    char pad0[0x18];
    int sub18;
    char pad1c[0x04];
    void* field20;
};

// captured call record
static void* cap_this;
static Trigger* cap_a1;
static void* cap_a2;
static void* cap_a3;
static Trigger* cap_a4;

static void* g_target = (void*)0xDEAD;

static void callee(void* self, Trigger* a1, void* a2, void* a3, Trigger* a4) {
    cap_this = self; cap_a1 = a1; cap_a2 = a2; cap_a3 = a3; cap_a4 = a4;
}

static void AddTrigger(Receptor* self, void* p1, Trigger* trigger) {
    callee(g_target, (Trigger*)p1, (void*)&self->sub18, self->field20, trigger);
}

int main() {
    Receptor r; r.sub18 = 7; r.field20 = (void*)0x1234;
    Trigger t; t.id = 99;
    Trigger p; p.id = 42;
    AddTrigger(&r, (void*)&p, &t);

    bool ok = cap_this == g_target
           && cap_a1 == (Trigger*)&p
           && cap_a2 == (void*)&r.sub18
           && cap_a3 == (void*)0x1234
           && cap_a4 == &t;
    if (ok) printf("BEHAVIOUR_OK\n");
    else printf("BEHAVIOUR_FAIL\n");
    return 0;
}