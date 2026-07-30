
#include <stdio.h>
#include <string.h>

// ---- Function under test (same lowering as source_cpp) ----
__declspec(naked) void UpdateThingAttachment()
{
    __asm {
        mov ecx, dword ptr [ecx+4]
        test ecx, ecx
        je  done
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax+0x11c]
    done:
        ret
    }
}

// ---- Test scaffolding ----
static int g_called = 0;
static void* g_seen_this = 0;

// The virtual method that lives at vtable slot 0x11c (offset 0x11c bytes).
// __fastcall: this in ecx. Record it.
static void __fastcall vfn(void* self, void* /*edx*/)
{
    g_called++;
    g_seen_this = self;
}

// A vtable large enough to hold slot at byte offset 0x11c.
static void* g_vtable[128];

struct Attachment {
    void** vptr;
};

struct ScriptThing {
    void* field0;      // [this+0]
    Attachment* attach; // [this+4]
};

// __fastcall shim: put obj in ecx, call the naked fn.
static void call_uta(void* obj)
{
    __asm {
        mov ecx, obj
        call UpdateThingAttachment
    }
}

int main()
{
    // --- Case 1: null attachment -> no call, no crash ---
    ScriptThing t1;
    t1.field0 = (void*)0xDEAD;
    t1.attach = 0;
    g_called = 0;
    call_uta(&t1);
    if (g_called != 0) { printf("FAIL null-path called=%d\n", g_called); return 1; }

    // --- Case 2: real attachment -> virtual slot 0x11c invoked with this=attach ---
    memset(g_vtable, 0, sizeof(g_vtable));
    g_vtable[0x11c/4] = (void*)&vfn;   // slot at byte offset 0x11c
    Attachment a;
    a.vptr = g_vtable;
    ScriptThing t2;
    t2.field0 = (void*)0xBEEF;
    t2.attach = &a;
    g_called = 0; g_seen_this = 0;
    call_uta(&t2);
    if (g_called != 1) { printf("FAIL call-path called=%d\n", g_called); return 1; }
    if (g_seen_this != (void*)&a) { printf("FAIL wrong-this %p != %p\n", g_seen_this, (void*)&a); return 1; }

    printf("OK_0x004ab0e0 null-skip=1 vcall-slot0x11c this-forwarded=1\n");
    return 0;
}