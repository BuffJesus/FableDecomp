#include <cstdio>

struct TCEntry {
    int key;
    void* iface;
};

struct VMap {
    TCEntry* begin;
    TCEntry* end;
    TCEntry* LowerBound(const int* key);
};

struct TargetT {
    unsigned char _pad0[0x2c];
    unsigned int flags2c;
    unsigned char _pad30[0x14];
    VMap vmap;
};

struct GSI;
typedef TargetT* (__fastcall *GetTargetFn)(GSI*);
struct GSIVtbl {
    unsigned char _pad[0x2c];
    GetTargetFn slot2c;
};
struct GSI {
    GSIVtbl* vt;
};

static TCEntry* g_hit;
TCEntry* VMap::LowerBound(const int* key) { (void)key; return g_hit; }

static int g_didCall;
static void* g_lastIface;
extern "C" void __fastcall Iface_Do(void* self) { g_lastIface = self; g_didCall++; }

bool __stdcall CGameScriptInterface_ManuallyTriggerTrap(GSI* self)
{
    TargetT* t = self->vt->slot2c(self);
    if (t && (t->flags2c & 0x20000000)) {
        VMap* vm = &t->vmap;
        int key = 0x7d;
        TCEntry* it = vm->LowerBound(&key);
        TCEntry* end = vm->end;
        if (it == end || it->key > 0x7d)
            it = end;
        Iface_Do(it->iface);
        return true;
    }
    return false;
}

static TargetT g_t;
static GSIVtbl g_vt;
static GSI g_self;

static TargetT* __fastcall retNull(GSI* s) { (void)s; return 0; }
static TargetT* __fastcall retT(GSI* s) { (void)s; return &g_t; }

int main()
{
    g_self.vt = &g_vt;

    g_vt.slot2c = retNull;
    g_didCall = 0;
    if (CGameScriptInterface_ManuallyTriggerTrap(&g_self)) { std::printf("FAIL null\n"); return 1; }
    if (g_didCall != 0) { std::printf("FAIL null-called\n"); return 1; }

    g_vt.slot2c = retT;
    g_t.flags2c = 0;
    g_didCall = 0;
    if (CGameScriptInterface_ManuallyTriggerTrap(&g_self)) { std::printf("FAIL flag\n"); return 1; }
    if (g_didCall != 0) { std::printf("FAIL flag-called\n"); return 1; }

    static TCEntry entries[2];
    entries[0].key = 0x7d; entries[0].iface = (void*)0x1234;
    entries[1].key = 0x80; entries[1].iface = (void*)0x5678;
    g_t.flags2c = 0x20000000;
    g_t.vmap.begin = &entries[0];
    g_t.vmap.end   = &entries[2];

    g_hit = &entries[0];
    g_didCall = 0; g_lastIface = 0;
    if (!CGameScriptInterface_ManuallyTriggerTrap(&g_self)) { std::printf("FAIL true\n"); return 1; }
    if (g_didCall != 1) { std::printf("FAIL notcalled\n"); return 1; }
    if (g_lastIface != (void*)0x1234) { std::printf("FAIL wrong-iface\n"); return 1; }

    g_hit = &entries[1];
    g_didCall = 0; g_lastIface = (void*)-1;
    if (!CGameScriptInterface_ManuallyTriggerTrap(&g_self)) { std::printf("FAIL true2\n"); return 1; }
    if (g_lastIface != g_t.vmap.end->iface) { std::printf("FAIL end-iface\n"); return 1; }

    g_hit = g_t.vmap.end;
    g_didCall = 0;
    if (!CGameScriptInterface_ManuallyTriggerTrap(&g_self)) { std::printf("FAIL true3\n"); return 1; }
    if (g_didCall != 1) { std::printf("FAIL notcalled3\n"); return 1; }

    std::printf("CGameScriptInterface_00895090_TEST PASS\n");
    return 0;
}