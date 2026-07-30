#include <stdio.h>
#include <string.h>

// The adjustor thunk under test: add ecx,8 ; jmp target.
// We supply the jump target ourselves and observe that (a) the thunk added 8
// to the incoming 'this' (ecx) and (b) it tail-jumped into our target.
static unsigned int g_seen_this = 0;
static int g_called = 0;

// Tail-call target. Receives adjusted 'this' in ecx (__fastcall captures it).
void __fastcall BaseImpl(void* thisptr)
{
    g_seen_this = (unsigned int)thisptr;
    g_called = 1;
}

extern "C" void BaseTransferBinaryIn_005dbe30(void);

__declspec(naked) void TransferBinaryIn()
{
    __asm {
        add ecx, 8
        jmp BaseTransferBinaryIn_005dbe30
    }
}

// Resolve the extern to our observable target.
extern "C" __declspec(naked) void BaseTransferBinaryIn_005dbe30(void)
{
    __asm { jmp BaseImpl }
}

int main()
{
    unsigned int fake_obj = 0x1000;   // pretend 'this'
    g_seen_this = 0; g_called = 0;

    __asm {
        mov ecx, fake_obj
        call TransferBinaryIn
    }

    if (!g_called) { printf("FAIL not called\n"); return 1; }
    if (g_seen_this != fake_obj + 8) { printf("FAIL adjust=%u\n", g_seen_this - fake_obj); return 1; }

    printf("OK_0x005dbe30 adjust=+8 this=0x%X\n", g_seen_this);
    return 0;
}