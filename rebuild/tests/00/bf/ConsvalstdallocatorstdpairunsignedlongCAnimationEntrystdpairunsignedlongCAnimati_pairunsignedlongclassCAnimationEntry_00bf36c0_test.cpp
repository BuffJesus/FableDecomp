#include <stdio.h>

// Behavioural model of ConsVal_Anim_A:
//   eax = ecx (this)
//   ecx = *(void**)0x1436ea8          (global holder pointer)
//   ecx = *(void**)(ecx + 0x6b0)      (dispatch object)
//   call target(dispatch_obj_this, orig_this, orig_edx)

static void* g_captured_callee_this;
static void* g_captured_a;
static void* g_captured_b;

static void model_target(void* callee_this, void* a, void* b)
{
    g_captured_callee_this = callee_this;
    g_captured_a = a;
    g_captured_b = b;
}

struct Holder { char pad[0x6b0]; void* obj; };

static void model_ConsVal_Anim_A(void* self, void* edx, Holder* holder)
{
    void* eax = self;
    void* dispatch = holder->obj;
    model_target(dispatch, eax, edx);
}

int main()
{
    Holder h;
    void* obj = (void*)0xDEAD1234;
    h.obj = obj;

    void* self = (void*)0x11110000;
    void* arg  = (void*)0x22220000;

    model_ConsVal_Anim_A(self, arg, &h);

    int ok = 1;
    if (g_captured_callee_this != obj) ok = 0;
    if (g_captured_a != self) ok = 0;
    if (g_captured_b != arg) ok = 0;

    void* self2 = (void*)0;
    void* arg2  = (void*)0x33330000;
    h.obj = (void*)0x44440000;
    model_ConsVal_Anim_A(self2, arg2, &h);
    if (g_captured_callee_this != (void*)0x44440000) ok = 0;
    if (g_captured_a != self2) ok = 0;
    if (g_captured_b != arg2) ok = 0;

    if (ok) printf("OK_00bf36c0\n");
    else    printf("FAIL_00bf36c0\n");
    return ok ? 0 : 1;
}