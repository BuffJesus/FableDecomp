#include <stdio.h>

// Mirror the layout the target relies on: object has a vtable pointer at +0,
// and the called method lives at vtable byte offset 0x5bc (slot 367).
struct FakeObj {
    void** vtbl;   // +0x00
};

struct CGuiWindow {
    char pad0[4];
    FakeObj* m_obj;   // +0x04
    char pad1[0x44];
    int m_id;         // +0x4c
};

// The authored function under test (its own header shape is internal).
void __fastcall CGuiWindow_OnResize(CGuiWindow* self);

static int g_called = 0;
static int g_a = -999;
static int g_b = -999;

// __fastcall target: ecx=self, args (a,b) on stack.
static void __fastcall target_impl(FakeObj* self, void* /*edx*/, int a, int b)
{
    g_called++;
    g_a = a;
    g_b = b;
}

int main()
{
    // vtable large enough to hold slot 367 (byte 0x5bc).
    static void* vt[400];
    vt[367] = (void*)&target_impl;

    FakeObj o;
    o.vtbl = vt;

    CGuiWindow w;
    w.m_obj = &o;

    // case 1: id == -1 -> no call
    w.m_id = -1;
    g_called = 0;
    CGuiWindow_OnResize(&w);
    int noCall = (g_called == 0);

    // case 2: id != -1 -> call with (id, 0)
    w.m_id = 42;
    g_called = 0;
    CGuiWindow_OnResize(&w);
    int called = (g_called == 1 && g_a == 42 && g_b == 0);

    if (noCall && called) {
        printf("RESIZE_OK id=%d a=%d b=%d\n", w.m_id, g_a, g_b);
        return 0;
    }
    printf("RESIZE_FAIL nc=%d c=%d\n", noCall, called);
    return 1;
}