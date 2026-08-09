#include <cstdio>

struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;

// Model the delegate as a raw vtable pointer object so the test is fully
// self-contained (no undefined base-class virtuals to link).
struct Delegate {
    void** vptr;
};

struct CTCCreatureNavigation {
    void* f0;
    Delegate* f4;
};

// The tested forwarder, modelled with a manual vtable dispatch that matches
// retail: ecx=this->f4, args (this, v) on the stack, slot 6 (offset 0x18).
typedef bool (__fastcall *Vfn6)(Delegate* self, int edx, CTCCreatureNavigation* orig, C3DVector* v);

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int edx, C3DVector* v)
{
    Delegate* d = self->f4;
    Vfn6 fn = (Vfn6)d->vptr[6];
    return fn(d, 0, self, v);
}

static Delegate* g_expect_self;
static CTCCreatureNavigation* g_expect_orig;
static C3DVector* g_expect_vec;
static bool g_ret;
static int g_hits;

static bool __fastcall Check(Delegate* self, int edx, CTCCreatureNavigation* orig, C3DVector* v)
{
    g_hits++;
    if (self != g_expect_self) { printf("BAD self\n"); return false; }
    if (orig != g_expect_orig) { printf("BAD orig\n"); return false; }
    if (v != g_expect_vec) { printf("BAD vec\n"); return false; }
    return g_ret;
}

int main()
{
    void* vt[16];
    for (int i = 0; i < 16; i++) vt[i] = 0;
    vt[6] = (void*)&Check;

    Delegate d;
    d.vptr = vt;

    CTCCreatureNavigation nav;
    nav.f0 = 0;
    nav.f4 = &d;

    C3DVector vec; vec.x = 1; vec.y = 2; vec.z = 3;

    g_expect_self = &d;
    g_expect_orig = &nav;
    g_expect_vec = &vec;
    g_hits = 0;

    g_ret = true;
    bool r1 = IsNewDestinationGoingToResetNavigation(&nav, 0, &vec);

    g_ret = false;
    bool r2 = IsNewDestinationGoingToResetNavigation(&nav, 0, &vec);

    if (r1 == true && r2 == false && g_hits == 2) {
        printf("OK_00c09e80\n");
        return 0;
    }
    printf("FAIL r1=%d r2=%d hits=%d\n", (int)r1, (int)r2, g_hits);
    return 1;
}