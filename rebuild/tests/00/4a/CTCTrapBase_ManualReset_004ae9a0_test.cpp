#include <stdio.h>

static volatile void* g_seen_this = 0;
static volatile int   g_seen_arg  = 0;
static volatile int   g_called    = 0;

extern "C" __declspec(naked) void FableTLC_ManualReset_Target(void)
{
    __asm {
        mov     eax, [esp+4]
        mov     g_seen_this, ecx
        mov     g_seen_arg, eax
        mov     dword ptr g_called, 1
        ret     4
    }
}

__declspec(naked) void ManualReset(int /*arg*/)
{
    __asm {
        mov     al, byte ptr [ecx+0x2662]
        test    al, al
        je      done
        add     ecx, 0x2010
        jmp     FableTLC_ManualReset_Target
    done:
        ret     4
    }
}

static void call_mr(void* self, int arg)
{
    __asm {
        mov     ecx, self
        mov     eax, arg
        push    eax
        call    ManualReset
    }
}

int main()
{
    static unsigned char obj[0x3000];
    int i;

    for (i = 0; i < 0x3000; ++i) obj[i] = 0;
    obj[0x2662] = 0;
    g_called = 0; g_seen_this = 0; g_seen_arg = 0;
    call_mr(obj, 0x1234);
    if (g_called != 0) { printf("FAIL: target called when flag=0\n"); return 1; }

    obj[0x2662] = 1;
    g_called = 0; g_seen_this = 0; g_seen_arg = 0;
    call_mr(obj, 0x5678);
    if (g_called != 1) { printf("FAIL: target not called when flag set\n"); return 1; }
    if (g_seen_this != (void*)(obj + 0x2010)) { printf("FAIL: bad this %p\n", g_seen_this); return 1; }
    if (g_seen_arg != 0x5678) { printf("FAIL: bad arg %x\n", g_seen_arg); return 1; }

    printf("OK_0x004ae9a0 ManualReset flag-gated forward this+=0x2010 verified\n");
    return 0;
}