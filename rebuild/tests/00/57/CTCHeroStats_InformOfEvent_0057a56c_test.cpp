#include <stdio.h>

// ---- function under test (identical to source_cpp) ----
__declspec(naked) void InformOfEvent()
{
    __asm {
        mov eax, dword ptr [esp+4]
        mov edx, dword ptr [eax]
        push ecx
        mov ecx, eax
        call dword ptr [edx]
        ret 4
    }
}

// The vtable slot 0 target: __fastcall(this in ecx, arg in edx pushed as stack -> here first stack param).
// Thunk does: push ecx (the forwarded event), mov ecx,obj, call [vtbl[0]].
// So the callee is a __fastcall/__fastcall: this=obj (ecx), stack arg = event.
static void* g_seen_this = 0;
static int   g_seen_event = 0;
static int   g_called = 0;

// __fastcall: this in ecx, next in edx, then stack. Only 'this' in ecx and one stack arg.
static void __fastcall slot0(void* self, int /*edx_unused*/, int event)
{
    g_seen_this  = self;
    g_seen_event = event;
    g_called++;
}

struct Obj {
    void* vtbl_ptr;   // +0x0 -> vtable
};

int main()
{
    // Build a vtable with slot0 first.
    void* vtbl[4];
    vtbl[0] = (void*)&slot0;
    Obj o;
    o.vtbl_ptr = (void*)&vtbl[0];

    int event = 0x1234;
    void* objp = &o;

    // Invoke thunk: ecx = event (forwarded), push objp (esp+4 stack arg), call InformOfEvent, ret 4 cleans it.
    __asm {
        mov ecx, event
        push objp
        call InformOfEvent
    }

    if (g_called != 1) { printf("FAIL called=%d\n", g_called); return 1; }
    if (g_seen_this != objp) { printf("FAIL this mismatch\n"); return 1; }
    if (g_seen_event != event) { printf("FAIL event=%x\n", g_seen_event); return 1; }

    printf("OK_0x0057a56c this=%p event=%x\n", g_seen_this, g_seen_event);
    return 0;
}