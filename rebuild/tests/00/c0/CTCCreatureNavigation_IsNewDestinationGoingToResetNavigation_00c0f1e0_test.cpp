#include <stdio.h>

struct C3DVector { float x, y, z; };

// Model the forwarder: calls f4's virtual slot 6 with (self, dst), returns bool.
struct Delegate;
struct Nav;

typedef bool (*VFn)(void* self, void* dst);

struct Delegate {
    VFn* vtbl;
};

struct Nav {
    void* pad;
    Delegate* f4;
};

static bool g_result;
static void* g_seen_self;
static void* g_seen_dst;

static bool vslot6(void* self, void* dst) {
    g_seen_self = self;
    g_seen_dst  = dst;
    return g_result;
}

static bool IsNewDestinationGoingToResetNavigation_model(Nav* self, C3DVector* dst, float f) {
    (void)f;
    Delegate* d = self->f4;
    return d->vtbl[6](self, dst);
}

int main() {
    VFn vt[8];
    for (int i = 0; i < 8; ++i) vt[i] = 0;
    vt[6] = &vslot6;

    Delegate d;
    d.vtbl = vt;

    Nav nav;
    nav.pad = 0;
    nav.f4 = &d;

    C3DVector v = {1.0f, 2.0f, 3.0f};

    g_result = true;
    bool r1 = IsNewDestinationGoingToResetNavigation_model(&nav, &v, 5.0f);
    if (!r1) { printf("FAIL true-branch\n"); return 1; }
    if (g_seen_self != (void*)&nav) { printf("FAIL self\n"); return 2; }
    if (g_seen_dst != (void*)&v)   { printf("FAIL dst\n"); return 3; }

    g_result = false;
    bool r2 = IsNewDestinationGoingToResetNavigation_model(&nav, &v, 0.0f);
    if (r2) { printf("FAIL false-branch\n"); return 4; }

    printf("OK_00c0f1e0\n");
    return 0;
}