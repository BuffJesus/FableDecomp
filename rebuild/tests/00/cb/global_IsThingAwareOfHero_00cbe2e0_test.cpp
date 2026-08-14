#include <stdio.h>

// Behaviour model of IsThingAwareOfHero: return Check() && Virtual0x40()
struct Thing;
typedef bool (*VFn)(Thing*);
struct Thing { VFn* vtbl; int checkResult; int slotResult; };

static bool helperCheck(Thing* t) { return t->checkResult != 0; }
static bool slot(Thing* t) { return t->slotResult != 0; }

static bool model(Thing* t) {
    if (!helperCheck(t)) return false;
    if (!slot(t)) return false;
    return true;
}

int main() {
    VFn vt[17];
    vt[16] = slot;

    Thing a; a.vtbl = vt; a.checkResult = 1; a.slotResult = 1;
    Thing b; b.vtbl = vt; b.checkResult = 0; b.slotResult = 1;
    Thing c; c.vtbl = vt; c.checkResult = 1; c.slotResult = 0;

    bool ok = true;
    ok &= (model(&a) == true);   // both true -> true
    ok &= (model(&b) == false);  // check false -> false, slot not called
    ok &= (model(&c) == false);  // check true, slot false -> false

    if (ok) printf("BEHAVIOUR_PASS\n");
    else    printf("BEHAVIOUR_FAIL\n");
    return 0;
}