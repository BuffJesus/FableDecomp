#include <cstdio>

static int g_freed = 0;
static int g_dtor = 0;
extern "C" void ext_free(void* p) { g_freed++; }

struct RefBlock {
    int count;
    void (__fastcall *dtor)(void*);
    void* arg;
};

struct CCountedPointer {
    void* ptr;
    RefBlock* ref;
};

static void __fastcall my_dtor(void* a) { g_dtor++; }

void __fastcall Dest_val(CCountedPointer* self)
{
    RefBlock* r = self->ref;
    if (r) {
        --r->count;
        r = self->ref;
        if (r->count == 0) {
            r->dtor(r->arg);
            ext_free(self->ref);
        }
    }
    self->ptr = 0;
    self->ref = 0;
}

int main() {
    // Case 1: count drops to 0 -> dtor + free called
    RefBlock rb; rb.count = 1; rb.dtor = my_dtor; rb.arg = (void*)0x1234;
    CCountedPointer cp; cp.ptr = (void*)0xABCD; cp.ref = &rb;
    Dest_val(&cp);
    if (g_dtor != 1 || g_freed != 1 || cp.ptr != 0 || cp.ref != 0 || rb.count != 0) {
        std::printf("FAIL case1\n"); return 1;
    }
    // Case 2: count > 1 -> no dtor/free
    RefBlock rb2; rb2.count = 3; rb2.dtor = my_dtor; rb2.arg = 0;
    CCountedPointer cp2; cp2.ptr = (void*)0x1; cp2.ref = &rb2;
    Dest_val(&cp2);
    if (g_dtor != 1 || g_freed != 1 || cp2.ptr != 0 || cp2.ref != 0 || rb2.count != 2) {
        std::printf("FAIL case2\n"); return 1;
    }
    // Case 3: null ref
    CCountedPointer cp3; cp3.ptr = (void*)0x99; cp3.ref = 0;
    Dest_val(&cp3);
    if (cp3.ptr != 0 || cp3.ref != 0) { std::printf("FAIL case3\n"); return 1; }
    std::printf("DestVal_00436740_TEST PASS\n");
    return 0;
}