// Behaviour model for CGameScriptInterface::AddGossipVillages @ 0x008aa330.
// Contract: copy-construct a temp from the by-value string, forward it (by value) to
// the impl which owns/destroys that copy, then destroy the thunk's own 'val' param.
// We check the observable call ordering.
#include <cstdio>

static int g_events[8];
static int g_n = 0;

struct CCharString {
    void* data;
    CCharString() { data = 0; }
    CCharString(const CCharString& o) { data = o.data; g_events[g_n++] = 1; } // 1 copyctor
    ~CCharString() { g_events[g_n++] = 3; }                                    // 3 dtor
};

struct CGameScriptInterface { int dummy; };

// impl takes the string BY VALUE and owns (destroys) it -> emits one dtor(3) for the
// copy it received.
static void Impl_AddGossipVillages(CCharString byVal) {
    g_events[g_n++] = 2; // 2 impl body
    (void)byVal;         // byVal destroyed here on scope exit (impl owns the copy)
}

static void AddGossipVillages(CGameScriptInterface* self, CCharString val) {
    (void)self;
    Impl_AddGossipVillages(val);
}

int main() {
    CGameScriptInterface gsi;
    gsi.dummy = 0;
    CCharString src;
    src.data = (void*)0x1234;
    g_n = 0;
    AddGossipVillages(&gsi, src);
    // Full expected sequence:
    //   1 : copyctor of 'val' (src -> val param)
    //   1 : copyctor of 'byVal' (val -> impl by-value arg)
    //   2 : impl body
    //   3 : dtor of impl's byVal copy (impl owns it)
    //   3 : dtor of thunk's 'val' parameter
    int ok = (g_n == 5 &&
              g_events[0] == 1 && g_events[1] == 1 && g_events[2] == 2 &&
              g_events[3] == 3 && g_events[4] == 3);
    if (ok) {
        printf("GOSSIP_ORDER_OK\n");
    } else {
        printf("FAIL n=%d seq=%d%d%d%d%d\n", g_n,
               g_events[0], g_events[1], g_events[2], g_events[3], g_events[4]);
    }
    return 0;
}