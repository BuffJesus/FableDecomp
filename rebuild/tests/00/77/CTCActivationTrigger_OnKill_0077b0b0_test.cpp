#include <stdio.h>

struct CTCActivationTrigger;

struct Sub {
    void destroy(CTCActivationTrigger* owner);
};

struct CTCActivationTrigger {
    char pad[0xc];
    Sub* fc;   // +0xc
};

static int g_destroyed = 0;
static CTCActivationTrigger* g_owner = 0;
static Sub* g_self = 0;

void Sub::destroy(CTCActivationTrigger* owner)
{
    g_destroyed++;
    g_owner = owner;
    g_self = this;
}

void __fastcall CTCActivationTrigger_OnKill(CTCActivationTrigger* self)
{
    if (self->fc) {
        self->fc->destroy(self);
        self->fc = 0;
    }
}

int main()
{
    Sub theSub;
    CTCActivationTrigger t;
    Sub* subp = &theSub;
    CTCActivationTrigger* tp = &t;
    t.fc = subp;

    CTCActivationTrigger_OnKill(tp);
    // fc had a value -> destroy called with owner=tp, this=subp, then fc=0
    int ok = (g_destroyed == 1) && (g_owner == tp) && (g_self == subp) && (t.fc == 0);

    // second call: fc==0 -> no-op
    CTCActivationTrigger_OnKill(tp);
    ok = ok && (g_destroyed == 1) && (t.fc == 0);

    if (ok) printf("0077b0b0_TEST PASS\n");
    else printf("0077b0b0_TEST FAIL d=%d\n", g_destroyed);
    return 0;
}