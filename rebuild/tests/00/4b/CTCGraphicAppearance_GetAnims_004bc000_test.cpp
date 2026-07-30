#include <stdio.h>

struct SubObj;
typedef void (__fastcall *AnimVFn)(SubObj* thisptr, int /*edx*/, void* a, void* b);
struct SubObj { AnimVFn* vtbl; };
struct CTCGraphicAppearance { char pad[0x24]; SubObj* subptr; };

// ---- function under test (naked transcription, identical to source) ----
extern "C" __declspec(naked) bool __fastcall GetAnims(void* thisptr, int edx, unsigned int arg)
{
    (void)thisptr; (void)edx; (void)arg;
    __asm {
        mov     eax, dword ptr [ecx+0x24]
        test    eax, eax
        je      miss
        mov     ecx, eax
        mov     eax, dword ptr [esp+0x4]
        mov     edx, dword ptr [ecx]
        push    esi
        lea     esi, dword ptr [eax+0xc]
        push    esi
        add     eax, 0x4
        push    eax
        call    dword ptr [edx+0x74]
        mov     al, 0x1
        pop     esi
        ret     0x4
    miss:
        xor     al, al
        ret     0x4
    }
}

// ---- test harness ----
static SubObj*  g_this;
static void*    g_a;
static void*    g_b;
static int      g_called;

static void __fastcall VFn(SubObj* thisptr, int /*edx*/, void* a, void* b)
{
    g_this = thisptr; g_a = a; g_b = b; g_called++;
}

static AnimVFn g_vtbl[32];

int main()
{
    // Case 1: subptr == null -> false, no vcall.
    CTCGraphicAppearance obj0;
    obj0.subptr = 0;
    g_called = 0;
    bool r0 = GetAnims(&obj0, 0, 0x1000);
    if (r0 != false || g_called != 0) {
        printf("FAIL null-case r=%d called=%d\n", (int)r0, g_called);
        return 1;
    }

    // Case 2: subptr set -> true, vslot 0x74 called with this=subptr, (arg+4, arg+0xc).
    for (int i = 0; i < 32; ++i) g_vtbl[i] = 0;
    g_vtbl[0x74 / 4] = VFn;
    SubObj node;
    node.vtbl = g_vtbl;
    CTCGraphicAppearance obj1;
    obj1.subptr = &node;

    g_called = 0; g_this = 0; g_a = 0; g_b = 0;
    unsigned int arg = 0x2000;
    bool r1 = GetAnims(&obj1, 0, arg);

    if (r1 != true || g_called != 1) {
        printf("FAIL hit-case r=%d called=%d\n", (int)r1, g_called);
        return 1;
    }
    if (g_this != &node) {
        printf("FAIL this got=%p want=%p\n", (void*)g_this, (void*)&node);
        return 1;
    }
    if (g_a != (void*)(arg + 4)) {
        printf("FAIL arg_a got=%p want=%p\n", g_a, (void*)(arg + 4));
        return 1;
    }
    if (g_b != (void*)(arg + 0xc)) {
        printf("FAIL arg_b got=%p want=%p\n", g_b, (void*)(arg + 0xc));
        return 1;
    }

    printf("OK_0x004bc000 called=%d a=%p b=%p\n", g_called, g_a, g_b);
    return 0;
}