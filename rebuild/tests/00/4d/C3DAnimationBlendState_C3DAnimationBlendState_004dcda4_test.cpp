#include <cstdio>

struct RefCounted {
    void** vtbl;
    int    refc;
};

struct C3DAnimationBlendState {
    char        pad[0x14];
    RefCounted* pRef;
};

static int g_baseDtorCalls = 0;
static int g_releaseCalls = 0;

void __fastcall base_dtor(void* self) { (void)self; g_baseDtorCalls++; }

void __fastcall releaseFn(RefCounted* p) { (void)p; g_releaseCalls++; }

void __fastcall dtor_C3DAnimationBlendState(C3DAnimationBlendState* self)
{
    RefCounted* p = self->pRef;
    if (p != 0) {
        if (--p->refc == 0) {
            typedef void (__fastcall *fn_t)(RefCounted*);
            ((fn_t)(p->vtbl[1]))(p);
        }
        self->pRef = 0;
    }
    base_dtor(self);
}

int main() {
    {
        C3DAnimationBlendState s; s.pRef = 0;
        g_baseDtorCalls = 0; g_releaseCalls = 0;
        dtor_C3DAnimationBlendState(&s);
        if (g_baseDtorCalls != 1 || g_releaseCalls != 0) { printf("FAIL1\n"); return 1; }
    }
    {
        void* vt[2]; vt[1] = (void*)&releaseFn;
        RefCounted r; r.vtbl = vt; r.refc = 2;
        C3DAnimationBlendState s; s.pRef = &r;
        g_baseDtorCalls = 0; g_releaseCalls = 0;
        dtor_C3DAnimationBlendState(&s);
        if (r.refc != 1 || g_releaseCalls != 0 || s.pRef != 0 || g_baseDtorCalls != 1) { printf("FAIL2\n"); return 1; }
    }
    {
        void* vt[2]; vt[1] = (void*)&releaseFn;
        RefCounted r; r.vtbl = vt; r.refc = 1;
        C3DAnimationBlendState s; s.pRef = &r;
        g_baseDtorCalls = 0; g_releaseCalls = 0;
        dtor_C3DAnimationBlendState(&s);
        if (r.refc != 0 || g_releaseCalls != 1 || s.pRef != 0 || g_baseDtorCalls != 1) { printf("FAIL3\n"); return 1; }
    }
    printf("BLENDSTATE_DTOR_OK\n");
    return 0;
}