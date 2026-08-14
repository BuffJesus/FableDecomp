#include <stdio.h>

struct RefCounted { void** vtable; int refCount; };
struct C3DAnimationBlendState { char pad0[0xc]; RefCounted* handle; };

static int g_destroyed = 0;
static int g_base_called = 0;

void __fastcall RC_destroy(RefCounted* h) { g_destroyed++; }
static void* g_vt[2];

void __fastcall Base_dtor_model(C3DAnimationBlendState* self) { g_base_called++; }

void __fastcall dtor(C3DAnimationBlendState* self)
{
    RefCounted* h = self->handle;
    if (h) {
        if (--h->refCount == 0) {
            typedef void (__fastcall *Fn)(RefCounted*);
            ((Fn)h->vtable[1])(h);
        }
        self->handle = 0;
    }
    Base_dtor_model(self);
}

int main()
{
    g_vt[0] = 0;
    g_vt[1] = (void*)&RC_destroy;

    // Case 1: refcount 1 -> hits zero -> destroy called, handle cleared, base called
    RefCounted rc1; rc1.vtable = (void**)g_vt; rc1.refCount = 1;
    C3DAnimationBlendState o1; o1.handle = &rc1;
    dtor(&o1);
    if (g_destroyed != 1) { printf("FAIL destroy1\n"); return 1; }
    if (o1.handle != 0) { printf("FAIL clear1\n"); return 1; }
    if (g_base_called != 1) { printf("FAIL base1\n"); return 1; }
    if (rc1.refCount != 0) { printf("FAIL rc1\n"); return 1; }

    // Case 2: refcount 2 -> no destroy, handle cleared, base called
    RefCounted rc2; rc2.vtable = (void**)g_vt; rc2.refCount = 2;
    C3DAnimationBlendState o2; o2.handle = &rc2;
    dtor(&o2);
    if (g_destroyed != 1) { printf("FAIL nodestroy2\n"); return 1; }
    if (rc2.refCount != 1) { printf("FAIL rc2\n"); return 1; }
    if (o2.handle != 0) { printf("FAIL clear2\n"); return 1; }
    if (g_base_called != 2) { printf("FAIL base2\n"); return 1; }

    // Case 3: null handle -> only base called
    C3DAnimationBlendState o3; o3.handle = 0;
    dtor(&o3);
    if (g_base_called != 3) { printf("FAIL base3\n"); return 1; }

    printf("PARITY_OK_004dc916\n");
    return 0;
}