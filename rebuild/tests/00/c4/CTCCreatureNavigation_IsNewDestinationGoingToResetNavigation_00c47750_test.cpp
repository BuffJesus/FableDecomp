#include <stdio.h>
struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;
struct Forwardee;

typedef bool (*SlotFn)(Forwardee* self, CTCCreatureNavigation* nav, C3DVector* dst);
struct ForwardeeVtbl { SlotFn slot7; };
struct Forwardee { ForwardeeVtbl* vtbl; };
struct CTCCreatureNavigation { void* f0; Forwardee* f4; };

static int g_called = 0;
static Forwardee* g_seen_fw = 0;
static CTCCreatureNavigation* g_seen_nav = 0;
static C3DVector* g_seen_dst = 0;
static bool g_ret = false;

static bool slotImpl(Forwardee* self, CTCCreatureNavigation* nav, C3DVector* dst) {
    g_called++;
    g_seen_fw = self;
    g_seen_nav = nav;
    g_seen_dst = dst;
    return g_ret;
}

static bool model(CTCCreatureNavigation* self, C3DVector* dst) {
    Forwardee* fw = self->f4;
    return fw->vtbl->slot7(fw, self, dst);
}

int main() {
    ForwardeeVtbl vt; vt.slot7 = slotImpl;
    Forwardee fw; fw.vtbl = &vt;
    CTCCreatureNavigation nav; nav.f0 = 0; nav.f4 = &fw;
    C3DVector dst; dst.x = 1; dst.y = 2; dst.z = 3;

    g_ret = true;
    bool r1 = model(&nav, &dst);
    if (!r1) { printf("FAIL ret true\n"); return 1; }
    if (g_called != 1) { printf("FAIL called\n"); return 1; }
    if (g_seen_fw != &fw) { printf("FAIL fw\n"); return 1; }
    if (g_seen_nav != &nav) { printf("FAIL nav\n"); return 1; }
    if (g_seen_dst != &dst) { printf("FAIL dst\n"); return 1; }

    g_ret = false;
    bool r2 = model(&nav, &dst);
    if (r2) { printf("FAIL ret false\n"); return 1; }
    if (g_called != 2) { printf("FAIL called2\n"); return 1; }

    printf("OK_00c47750\n");
    return 0;
}