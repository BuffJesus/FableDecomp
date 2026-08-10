#include <cstdio>

struct C3DVector { float x,y,z; };
struct Inner;
struct CTCCreatureNavigation { Inner* f0; Inner* f4; };

// vtable-slot-7 signature: this=inner, arg1=outer, arg2=c3dvector
typedef bool (__fastcall *Slot7)(void* inner, int edx, void* outer, C3DVector* v);

static int g_calls = 0;
static void* g_seen_inner = 0;
static void* g_seen_outer = 0;
static C3DVector* g_seen_vec = 0;
static bool g_ret = false;

static bool __fastcall slot7_impl(void* inner, int /*edx*/, void* outer, C3DVector* v)
{
    g_calls++;
    g_seen_inner = inner;
    g_seen_outer = outer;
    g_seen_vec = v;
    return g_ret;
}

struct Inner { void* vtbl_pad[7]; void* slot7; };

// model matching the naked forwarder byte-for-byte
static bool model(CTCCreatureNavigation* self, C3DVector* dest, float /*dist*/)
{
    Inner* inner = self->f4;               // ecx = [this+4]
    void** vt = *(void***)inner;           // edx = [ecx]
    Slot7 fn = (Slot7)vt[7];               // [edx+0x1c]
    return fn(inner, 0, self, dest);       // this=inner, arg1=outer, arg2=dest
}

int main()
{
    // build a fake vtable with slot 7 populated
    void* vt[8] = {0,0,0,0,0,0,0,(void*)&slot7_impl};
    struct FakeInner { void** vp; } fi;
    fi.vp = vt;

    CTCCreatureNavigation nav;
    nav.f0 = 0;
    nav.f4 = (Inner*)&fi;

    C3DVector v; v.x=1; v.y=2; v.z=3;

    g_ret = true;
    bool r1 = model(&nav, &v, 5.0f);
    if (!r1 || g_calls!=1 || g_seen_inner!=(void*)&fi || g_seen_outer!=(void*)&nav || g_seen_vec!=&v) {
        printf("FAIL branch1\n"); return 1;
    }

    g_ret = false;
    bool r2 = model(&nav, &v, 9.0f);
    if (r2 || g_calls!=2) { printf("FAIL branch2\n"); return 1; }

    printf("OK_00c367e0\n");
    return 0;
}