#include <cstdio>
#include <cstdlib>

// ---- Function under test: CRenderTarget::DetachTarget @ 0x004c7590 ----
// __fastcall forwarder. this in ECX, one 4-byte stack arg.

static int g_calls = 0;      // how many times the child forwarder ran
static void* g_seen_this = 0; // ECX the child saw
static void* g_seen_arg  = 0; // stack arg the child saw

// External tail-call target (child's DetachTarget). __fastcall so this=ECX.
extern "C" void __fastcall CRenderTarget_ChildDetach(void* self, void* edx, void* arg)
{
    (void)edx;
    g_calls++;
    g_seen_this = self;
    g_seen_arg  = arg;
}

__declspec(naked) void __fastcall DetachTarget(void* thisptr, void* edx, void* arg)
{
    (void)thisptr; (void)edx; (void)arg;
    __asm {
        mov ecx, dword ptr [ecx+0x64]
        test ecx, ecx
        je short done
        jmp CRenderTarget_ChildDetach
    done:
        ret 4
    }
}

// Object layout: only field at +0x64 matters (the child pointer).
struct RT { char pad[0x64]; void* child; };

// __asm shim: put this in ECX, push the single stack arg, call, and
// (callee does ret 4) balance is handled by the callee's ret 4.
static void call_detach(RT* self, void* arg)
{
    void* p = self;
    __asm {
        push arg
        mov ecx, p
        call DetachTarget
    }
}

int main()
{
    // Case 1: child is null -> forwarder must NOT be called.
    RT a; a.child = 0;
    g_calls = 0; g_seen_this = 0; g_seen_arg = 0;
    call_detach(&a, (void*)0xCAFE1111);
    if (g_calls != 0) { printf("FAIL null-child called forwarder\n"); return 1; }

    // Case 2: child non-null -> forwarder called with this=child, same arg.
    RT parent; RT kid; kid.child = 0;
    parent.child = &kid;
    g_calls = 0; g_seen_this = 0; g_seen_arg = 0;
    call_detach(&parent, (void*)0xBEEF2222);
    if (g_calls != 1)              { printf("FAIL non-null not forwarded (calls=%d)\n", g_calls); return 1; }
    if (g_seen_this != (void*)&kid){ printf("FAIL wrong this forwarded\n"); return 1; }
    if (g_seen_arg != (void*)0xBEEF2222){ printf("FAIL wrong arg forwarded\n"); return 1; }

    printf("OK_0x004c7590\n");
    return 0;
}