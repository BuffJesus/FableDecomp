#include <cstdio>
struct RefCounted {
    int count;
    void (__fastcall *dtor)(void*);
    void* arg;
};
struct CCountedPointer {
    void* self;
    RefCounted* p;
};
static int g_dtor_called = 0;
static int g_freed = 0;
static RefCounted* g_expect_free = 0;
extern "C" void __cdecl engine_free(void* q) { g_freed = 1; if ((void*)g_expect_free == q) g_freed = 2; }
static void __fastcall my_dtor(void* a) { g_dtor_called = 1; }

void __fastcall _Dest_val(CCountedPointer* self)
{
    RefCounted* p = self->p;
    if (p) {
        --p->count;
        p = self->p;
        if (p->count == 0) {
            p->dtor(p->arg);
            engine_free(self->p);
        }
    }
    self->self = 0;
    self->p = 0;
}

int main() {
    /* Case 1: count goes to zero -> dtor + free */
    RefCounted rc; rc.count = 1; rc.dtor = my_dtor; rc.arg = (void*)0;
    CCountedPointer cp; cp.self = (void*)0x1234; cp.p = &rc;
    g_expect_free = &rc;
    _Dest_val(&cp);
    if (!(g_dtor_called == 1 && g_freed == 2 && cp.self == 0 && cp.p == 0 && rc.count == 0)) { std::printf("FAIL case1\n"); return 1; }

    /* Case 2: count stays > 0 -> no dtor, no free */
    g_dtor_called = 0; g_freed = 0;
    RefCounted rc2; rc2.count = 3; rc2.dtor = my_dtor; rc2.arg = (void*)0;
    CCountedPointer cp2; cp2.self = (void*)0x5; cp2.p = &rc2;
    _Dest_val(&cp2);
    if (!(g_dtor_called == 0 && g_freed == 0 && cp2.self == 0 && cp2.p == 0 && rc2.count == 2)) { std::printf("FAIL case2\n"); return 1; }

    /* Case 3: null pointer -> just clears */
    g_dtor_called = 0; g_freed = 0;
    CCountedPointer cp3; cp3.self = (void*)0x9; cp3.p = 0;
    _Dest_val(&cp3);
    if (!(g_dtor_called == 0 && g_freed == 0 && cp3.self == 0 && cp3.p == 0)) { std::printf("FAIL case3\n"); return 1; }

    std::printf("DestVal_0041bb30_TEST PASS\n");
    return 0;
}