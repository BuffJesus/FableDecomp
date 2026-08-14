#include <cstdio>

struct RefBlock {
    long count;
    void (__fastcall *dtor)(void* self, void* edx, int flags);
    void* obj;
};

static int g_freed = 0;
static int g_dtored = 0;

extern "C" void __cdecl EngineFree(void* p) { (void)p; ++g_freed; }
static void __fastcall fake_dtor(void* self, void* edx, int flags) {
    (void)self;(void)edx;(void)flags; ++g_dtored;
}

// Re-implemented logic mirror of the release path.
static void release(RefBlock* blk) {
    if (blk) {
        if (--blk->count == 0) {
            blk->dtor(blk->obj, 0, 0);
            EngineFree(blk);
        }
    }
}

int main() {
    // Case 1: null -> nothing
    release(0);

    // Case 2: count 2 -> becomes 1, no free
    RefBlock a; a.count = 2; a.dtor = fake_dtor; a.obj = &a;
    release(&a);
    if (a.count != 1 || g_freed != 0 || g_dtored != 0) { printf("FAIL c2\n"); return 1; }

    // Case 3: count 1 -> becomes 0, dtor+free fire
    RefBlock b; b.count = 1; b.dtor = fake_dtor; b.obj = &b;
    release(&b);
    if (b.count != 0 || g_freed != 1 || g_dtored != 1) { printf("FAIL c3\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}