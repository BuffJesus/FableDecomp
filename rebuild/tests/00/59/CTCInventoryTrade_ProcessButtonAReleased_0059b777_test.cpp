#include <cstdio>

// ---- function-under-test: CTCInventoryTrade::ProcessButtonAReleased @0x0059b777
// __fastcall. If field +0x1c == 1, dispatch vtable slot [0x110]; then tail-jmp
// the button-A helper with this preserved. Trailing thunks are adjacent slices.
extern "C" void ProcessButtonAReleased_tail(void);
extern "C" void ProcessButtonAReleased_thunk1(void);
extern "C" void ProcessButtonAReleased_thunk2(void);

__declspec(naked) void ProcessButtonAReleased()
{
    __asm {
        push esi
        mov  esi, ecx
        cmp  dword ptr [esi+0x1c], 1
        jne  skip
        mov  eax, dword ptr [esi]
        call dword ptr [eax+0x110]
    skip:
        mov  ecx, esi
        pop  esi
        jmp  ProcessButtonAReleased_tail
        jmp  ProcessButtonAReleased_thunk1
        jmp  ProcessButtonAReleased_thunk2
    }
}

// ---- behavioural harness -------------------------------------------------
// Observed side effects:
static void*        g_slotThis   = 0;  // this seen by vtable slot [0x110]
static int          g_slotCalls  = 0;
static void*        g_tailThis   = 0;  // this seen by the tail helper
static int          g_tailCalls  = 0;

// Object layout: vtable at +0x00, mode flag at +0x1c.
struct Obj {
    void** vtbl;   // +0x00
    int    pad04;  // +0x04
    int    pad08;  // +0x08
    int    pad0c;  // +0x0c
    int    pad10;  // +0x10
    int    pad14;  // +0x14
    int    pad18;  // +0x18
    int    mode;   // +0x1c
};

// vtable slot at byte offset 0x110 => index 0x110/4 = 68. __fastcall: this=ecx.
static void __declspec(naked) VSlot110()
{
    __asm {
        mov  g_slotThis, ecx
        inc  g_slotCalls
        ret
    }
}

// The tail helper: records this(=ecx) then returns to the ORIGINAL caller,
// because ProcessButtonAReleased reached it via 'jmp' with a balanced frame.
extern "C" __declspec(naked) void ProcessButtonAReleased_tail(void)
{
    __asm {
        mov  g_tailThis, ecx
        inc  g_tailCalls
        ret
    }
}
// Unreached in these tests, but must resolve for the standalone link.
extern "C" __declspec(naked) void ProcessButtonAReleased_thunk1(void) { __asm { ret } }
extern "C" __declspec(naked) void ProcessButtonAReleased_thunk2(void) { __asm { ret } }

// __fastcall shim: this=ecx, no stack args, no return value.
static void call_fut(Obj* self)
{
    void (*fn)() = (void(*)())&ProcessButtonAReleased;
    __asm {
        mov  ecx, self
        call fn
    }
}

int main()
{
    // Build a vtable big enough to cover slot index 68.
    void* vt[80];
    for (int i = 0; i < 80; ++i) vt[i] = 0;
    vt[0x110/4] = (void*)&VSlot110;

    Obj a;  a.vtbl = vt;  a.mode = 1;   // active mode -> should dispatch slot
    Obj b;  b.vtbl = vt;  b.mode = 0;   // inactive    -> skip slot dispatch

    int rc = 0;

    // Case 1: mode == 1 -> vtable slot invoked with this=&a, then tail with this=&a.
    g_slotCalls = g_tailCalls = 0; g_slotThis = g_tailThis = 0;
    call_fut(&a);
    if (g_slotCalls != 1)        { printf("FAIL mode1: slot not called (%d)\n", g_slotCalls); rc = 1; }
    if (g_slotThis  != (void*)&a){ printf("FAIL mode1: slot this=%p\n", g_slotThis); rc = 1; }
    if (g_tailCalls != 1)        { printf("FAIL mode1: tail not called (%d)\n", g_tailCalls); rc = 1; }
    if (g_tailThis  != (void*)&a){ printf("FAIL mode1: tail this=%p\n", g_tailThis); rc = 1; }

    // Case 2: mode != 1 -> slot skipped, tail still runs with this=&b.
    g_slotCalls = g_tailCalls = 0; g_slotThis = g_tailThis = 0;
    call_fut(&b);
    if (g_slotCalls != 0)        { printf("FAIL mode0: slot wrongly called (%d)\n", g_slotCalls); rc = 1; }
    if (g_tailCalls != 1)        { printf("FAIL mode0: tail not called (%d)\n", g_tailCalls); rc = 1; }
    if (g_tailThis  != (void*)&b){ printf("FAIL mode0: tail this=%p\n", g_tailThis); rc = 1; }

    // Case 3: boundary — mode==2 must also skip (only exactly 1 dispatches).
    Obj c; c.vtbl = vt; c.mode = 2;
    g_slotCalls = g_tailCalls = 0;
    call_fut(&c);
    if (g_slotCalls != 0)        { printf("FAIL mode2: slot wrongly called (%d)\n", g_slotCalls); rc = 1; }
    if (g_tailCalls != 1)        { printf("FAIL mode2: tail not called (%d)\n", g_tailCalls); rc = 1; }

    if (rc == 0) { printf("OK_0x0059b777 ProcessButtonAReleased behaviour verified\n"); return 0; }
    return 1;
}