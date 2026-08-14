// Behaviour model for C3DAnimationBlendState::~C3DAnimationBlendState
#include <cstdio>

struct RefCounted {
    void** vtbl;
    int    refs;
};

static int g_released = 0;
static int g_base_ran = 0;

static void __fastcall release_impl(RefCounted* p) {
    (void)p;
    g_released++;
}

// vtable with slot1 = release_impl
static void* g_vtbl[2] = { 0, (void*)&release_impl };

struct State {
    char        pad[0x14];
    RefCounted* ptr;
};

static void base_dtor(State* self) {
    (void)self;
    g_base_ran++;
}

static void dtor(State* self) {
    RefCounted* p = self->ptr;
    if (p) {
        if (--p->refs == 0) {
            typedef void (__fastcall *VF)(RefCounted*);
            ((VF)p->vtbl[1])(p);
        }
        self->ptr = 0;
    }
    base_dtor(self);
}

int main() {
    // Case A: refs>1 -> decrement only, no release, ptr cleared, base ran
    RefCounted rc1; rc1.vtbl = g_vtbl; rc1.refs = 2;
    State s1; s1.ptr = &rc1;
    dtor(&s1);
    bool okA = (rc1.refs == 1) && (g_released == 0) && (s1.ptr == 0) && (g_base_ran == 1);

    // Case B: refs==1 -> release called, ptr cleared, base ran
    RefCounted rc2; rc2.vtbl = g_vtbl; rc2.refs = 1;
    State s2; s2.ptr = &rc2;
    dtor(&s2);
    bool okB = (g_released == 1) && (s2.ptr == 0) && (g_base_ran == 2);

    // Case C: null ptr -> nothing but base
    State s3; s3.ptr = 0;
    dtor(&s3);
    bool okC = (g_released == 1) && (s3.ptr == 0) && (g_base_ran == 3);

    if (okA && okB && okC) {
        printf("BLENDSTATE_DTOR_OK\n");
    } else {
        printf("FAIL A=%d B=%d C=%d\n", okA, okB, okC);
    }
    return 0;
}