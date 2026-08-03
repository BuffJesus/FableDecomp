
#include <stdio.h>

// ---- function under test (same naked-asm body) ----
__declspec(naked) void RemoveItemFromInventory()
{
    __asm {
        mov     edx, dword ptr [esp+4]
        movzx   edx, word ptr [edx+0x8c]
        mov     eax, dword ptr [ecx]
        push    1
        push    edx
        call    dword ptr [eax+0x13c]
        ret     4
    }
}

// ---- test scaffolding ----
struct Item { char pad[0x8c]; unsigned short id; };

// Object with a fake vtable large enough to hold slot 0x13c.
// vtable is an array of function pointers; slot index 0x13c/4 = 79.
static unsigned int g_capturedArg = 0xFFFFFFFF;
static int          g_capturedFlag = -1;
static void*        g_capturedThis = 0;

// The virtual method: __fastcall (this=ecx), stack args (unsigned int id, int flag).
// Callee cleans its 2 stack args (ret 8). Naked shim captures ecx + both args.
__declspec(naked) static void vf()
{
    __asm {
        mov     eax, dword ptr [esp+4]   ; id (first pushed-last arg)
        mov     dword ptr [g_capturedArg], eax
        mov     eax, dword ptr [esp+8]   ; flag
        mov     dword ptr [g_capturedFlag], eax
        mov     dword ptr [g_capturedThis], ecx
        ret     8
    }
}

int main()
{
    // Build a vtable with slot 79 pointing at vf.
    static void* vtbl[80];
    for (int i = 0; i < 80; ++i) vtbl[i] = 0;
    vtbl[0x13c / 4] = (void*)&vf;

    // Object: first field is the vtable pointer.
    struct Obj { void** vptr; } obj;
    obj.vptr = vtbl;

    Item item;
    item.id = 0x1234;

    void* pobj = &obj;
    Item* pitem = &item;
    __asm {
        mov  ecx, pobj
        push pitem
        call RemoveItemFromInventory
    }

    if (g_capturedThis != &obj) { printf("FAIL this\n"); return 1; }
    if (g_capturedArg != 0x1234) { printf("FAIL arg=%x\n", g_capturedArg); return 1; }
    if (g_capturedFlag != 1)     { printf("FAIL flag=%d\n", g_capturedFlag); return 1; }

    // Second case: different id, verify zero-extension of u16.
    g_capturedThis = 0; g_capturedArg = 0; g_capturedFlag = -1;
    item.id = 0xFFFF;
    __asm {
        mov  ecx, pobj
        push pitem
        call RemoveItemFromInventory
    }
    if (g_capturedArg != 0xFFFF) { printf("FAIL arg2=%x\n", g_capturedArg); return 1; }
    if (g_capturedFlag != 1)     { printf("FAIL flag2\n"); return 1; }

    printf("OK_0x005bc499\n");
    return 0;
}