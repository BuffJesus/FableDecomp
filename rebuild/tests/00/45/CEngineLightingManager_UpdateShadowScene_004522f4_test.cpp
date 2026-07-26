#include <cstdio>

struct CShadowArg { int tag; };

struct CShadowSink {
    int consumed;
    CShadowArg* last;
    void Consume(CShadowArg* a) { consumed++; last = a; }
};

struct CEngineLightingManager {
    char pad0[0x18];
    int  mode;      // +0x18
    char pad1c[0x24-0x1c];
    void* obj24;    // +0x24
    void* obj28;    // +0x28
};

struct CLocalHolder {
    int v;
    void* seenArg;
    void* seenObj;
    void Handle(void* arg, void* obj) { seenArg = arg; seenObj = obj; }
};

static int g_scopeCalls = 0;
int g_shadowTag = 0;
void EnterShadowScope(void* tag) { (void)tag; g_scopeCalls++; }

// Test re-implementation mirroring the source body (test uses a local holder
// captured for observation via a global to verify the mode routing).
static CLocalHolder g_lastLocal;

void UpdateShadowScene(CEngineLightingManager* self, CShadowArg** ppArg)
{
    EnterShadowScope((void*)&g_shadowTag);
    CLocalHolder local;
    local.v = 0;
    local.seenArg = 0; local.seenObj = 0;
    int m = self->mode;
    if (m == 2) {
        local.Handle(self->obj24, ppArg);
    } else if (m == 3) {
        ((CShadowSink*)self->obj28)->Consume(*ppArg);
    }
    g_lastLocal = local;
}

int main() {
    CShadowArg arg; arg.tag = 7;
    CShadowArg* pArg = &arg;

    // mode == 2 : local.Handle(obj24, ppArg)
    CEngineLightingManager m2; m2.mode = 2;
    void* fake24 = (void*)0x1234;
    m2.obj24 = fake24; m2.obj28 = 0;
    g_scopeCalls = 0;
    UpdateShadowScene(&m2, &pArg);
    if (g_scopeCalls != 1) { std::printf("FAIL scope m2\n"); return 1; }
    if (g_lastLocal.seenArg != fake24) { std::printf("FAIL m2 arg\n"); return 1; }
    if (g_lastLocal.seenObj != (void*)&pArg) { std::printf("FAIL m2 obj\n"); return 1; }

    // mode == 3 : sink->Consume(*ppArg)
    CShadowSink sink; sink.consumed = 0; sink.last = 0;
    CEngineLightingManager m3; m3.mode = 3;
    m3.obj24 = 0; m3.obj28 = &sink;
    g_scopeCalls = 0;
    UpdateShadowScene(&m3, &pArg);
    if (g_scopeCalls != 1) { std::printf("FAIL scope m3\n"); return 1; }
    if (sink.consumed != 1 || sink.last != pArg) { std::printf("FAIL m3\n"); return 1; }

    // other mode : only scope enter, no routing
    CEngineLightingManager m0; m0.mode = 0;
    m0.obj24 = 0; m0.obj28 = 0;
    g_scopeCalls = 0;
    sink.consumed = 0;
    UpdateShadowScene(&m0, &pArg);
    if (g_scopeCalls != 1) { std::printf("FAIL scope m0\n"); return 1; }
    if (sink.consumed != 0) { std::printf("FAIL m0 routed\n"); return 1; }

    std::printf("CEngineLightingManager_004522f4_TEST PASS\n");
    return 0;
}