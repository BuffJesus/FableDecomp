#include <cstdio>

// Behaviour model of CMiniMapDisplay::InitialiseOnRegionLoad.
// Verifies control flow: (1) bail when gate is true, (2) full chain when false.

static int g_gateReturns = 0;
static int g_subCalled = 0;
static int g_f4fb150Called = 0;
static int g_f4fc180Called = 0;
static int g_finalCalled = 0;
static void *g_finalArgA = 0;
static void *g_finalCs = 0;
static int g_finalFlag = 0;

struct Vtbl {
    int (*gate)();
    void * (*slot34)();
};

static void *g_subObjMarker = (void *)0xABCD;
static void *g_slot34Ret    = (void *)0x1111;
static void *g_f4fb150Ret   = (void *)0x2222;
static void *g_f4fc180Ret   = (void *)0x3333;

static int gate() { return g_gateReturns; }
static void *slot34() { g_subCalled = 1; return g_slot34Ret; }
static void *f4fb150(void *self) { g_f4fb150Called = 1; return g_f4fb150Ret; }
static void *f4fc180(void *self, void *a) { g_f4fc180Called = 1; return g_f4fc180Ret; }
static void final(void *self, void *a, void *cs, int flag) {
    g_finalCalled = 1; g_finalArgA = a; g_finalCs = cs; g_finalFlag = flag;
}

static Vtbl gv = { gate, slot34 };

static void model(void *self, void *cs)
{
    if (gate()) return;
    void *obj = g_slot34Ret; // sub->slot34()
    g_subCalled = 1; (void)obj;
    void *a = f4fb150(obj);
    void *b = f4fc180(obj, a);
    final(self, b, cs, 1);
}

int main()
{
    void *self = (void *)0x9999;
    void *cs   = (void *)0x8888;

    // Case 1: gate true -> bail, nothing else runs.
    g_gateReturns = 1;
    g_subCalled = g_f4fb150Called = g_f4fc180Called = g_finalCalled = 0;
    model(self, cs);
    if (g_subCalled || g_f4fb150Called || g_f4fc180Called || g_finalCalled) {
        printf("FAIL_bail\n"); return 1;
    }

    // Case 2: gate false -> full chain.
    g_gateReturns = 0;
    g_subCalled = g_f4fb150Called = g_f4fc180Called = g_finalCalled = 0;
    model(self, cs);
    if (!g_subCalled || !g_f4fb150Called || !g_f4fc180Called || !g_finalCalled) {
        printf("FAIL_chain\n"); return 1;
    }
    if (g_finalArgA != g_f4fc180Ret || g_finalCs != cs || g_finalFlag != 1) {
        printf("FAIL_args\n"); return 1;
    }

    printf("PARITY_OK_0082a180\n");
    return 0;
}