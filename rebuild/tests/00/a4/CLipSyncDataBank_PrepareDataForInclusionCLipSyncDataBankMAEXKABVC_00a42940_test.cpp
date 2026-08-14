#include <stdio.h>

struct Ref {
    long refCount;
    void (__fastcall *dtor)(void* self);
    void* object;
};

static int g_dtorCalls = 0;
static int g_freeCalls = 0;
static void* g_lastDtorSelf = 0;

static void __fastcall myDtor(void* self) { g_dtorCalls++; g_lastDtorSelf = self; }

static void relFree(Ref* p) { g_freeCalls++; (void)p; }

// Behaviour model mirroring the reconstruction.
static void release(Ref* ref) {
    if (ref) {
        if (--ref->refCount == 0) {
            ref->dtor(ref->object);
            relFree(ref);
        }
    }
}

int main() {
    int obj = 0;
    Ref r;

    // NULL: no-op
    release(0);
    if (g_dtorCalls != 0 || g_freeCalls != 0) { printf("FAIL null\n"); return 1; }

    // refcount > 1: decrement, no destroy
    r.refCount = 3; r.dtor = myDtor; r.object = &obj;
    release(&r);
    if (r.refCount != 2 || g_dtorCalls != 0 || g_freeCalls != 0) { printf("FAIL dec\n"); return 1; }

    // refcount == 1: decrement to 0 -> destroy + free
    r.refCount = 1;
    release(&r);
    if (r.refCount != 0 || g_dtorCalls != 1 || g_freeCalls != 1 || g_lastDtorSelf != &obj) {
        printf("FAIL destroy\n"); return 1;
    }

    printf("PARITY_OK\n");
    return 0;
}