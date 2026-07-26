#include <cstdio>

extern "C" void __cdecl fable_op_delete(void* p);

struct CtrlBlock {
    long refcount;
    void (__fastcall* dtor)(void* self);
    void* obj;
};

struct Pair {
    unsigned long first;
    void* dummy;
    CtrlBlock* ctrl;
};

static int g_deleted = 0;
static int g_dtor_called = 0;
static void* g_dtor_arg = 0;

extern "C" void __cdecl fable_op_delete(void* p) { g_deleted = 1; }

static void __fastcall my_dtor(void* self) { g_dtor_called = 1; g_dtor_arg = self; }

void __fastcall Dest_val(Pair* self)
{
    CtrlBlock* cb = self->ctrl;
    if (cb != 0) {
        --cb->refcount;
        cb = self->ctrl;
        if (cb->refcount == 0) {
            cb->dtor(cb->obj);
            fable_op_delete(self->ctrl);
        }
    }
    self->dummy = 0;
    self->ctrl = 0;
}

int main() {
    /* Case 1: refcount drops to 0 -> dtor + delete run */
    CtrlBlock cb; cb.refcount = 1; cb.dtor = my_dtor; cb.obj = (void*)0x1234;
    Pair p; p.first = 5; p.dummy = (void*)0xdead; p.ctrl = &cb;
    Dest_val(&p);
    if (!(g_dtor_called == 1 && g_dtor_arg == (void*)0x1234 && g_deleted == 1 && p.dummy == 0 && p.ctrl == 0 && cb.refcount == 0)) {
        std::printf("FAIL case1\n"); return 1;
    }

    /* Case 2: refcount > 1 -> only decrement, no dtor/delete */
    g_dtor_called = 0; g_deleted = 0;
    CtrlBlock cb2; cb2.refcount = 3; cb2.dtor = my_dtor; cb2.obj = (void*)0x2222;
    Pair p2; p2.first = 7; p2.dummy = (void*)0xbeef; p2.ctrl = &cb2;
    Dest_val(&p2);
    if (!(g_dtor_called == 0 && g_deleted == 0 && cb2.refcount == 2 && p2.dummy == 0 && p2.ctrl == 0)) {
        std::printf("FAIL case2\n"); return 1;
    }

    /* Case 3: null ctrl -> just clears fields */
    g_dtor_called = 0; g_deleted = 0;
    Pair p3; p3.first = 9; p3.dummy = (void*)0x1; p3.ctrl = 0;
    Dest_val(&p3);
    if (!(g_dtor_called == 0 && g_deleted == 0 && p3.dummy == 0 && p3.ctrl == 0)) {
        std::printf("FAIL case3\n"); return 1;
    }

    std::printf("DestVal_00447e00_TEST PASS\n");
    return 0;
}