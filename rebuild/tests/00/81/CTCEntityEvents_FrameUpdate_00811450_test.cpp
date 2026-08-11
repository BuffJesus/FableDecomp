#include <cstdio>

static int g_baseCalled = 0;
static int g_subArg = -999;
static void* g_baseThis = 0;
static void* g_subThis = 0;

struct CBaseEntityEvents {
    void baseUpdate();
};
struct CSubObject {
    void consume(int x);
};
extern int helper_49d870(void);

struct CTCEntityEvents {
    CBaseEntityEvents base;
    char pad[0x0c - sizeof(CBaseEntityEvents)];
    CSubObject sub;
};

void __fastcall CTCEntityEvents_FrameUpdate(CTCEntityEvents* self);

// definitions of externs for the test
void CBaseEntityEvents::baseUpdate() { g_baseCalled++; g_baseThis = this; }
void CSubObject::consume(int x) { g_subArg = x; g_subThis = this; }
int helper_49d870(void) { return 4242; }

int main() {
    CTCEntityEvents obj;
    CTCEntityEvents_FrameUpdate(&obj);
    bool ok = (g_baseCalled == 1)
        && (g_subArg == 4242)
        && (g_baseThis == (void*)&obj)
        && (g_subThis == (void*)&obj.sub);
    if (ok) printf("00811450_TEST PASS\n");
    else printf("FAIL base=%d arg=%d\n", g_baseCalled, g_subArg);
    return 0;
}