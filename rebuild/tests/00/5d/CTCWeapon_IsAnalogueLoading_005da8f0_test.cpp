#include <stdio.h>

__declspec(naked) unsigned char IsAnalogueLoading()
{
    __asm
    {
        mov  ecx, [ecx+0x0c]
        test ecx, ecx
        je   skip_inc
        inc  dword ptr [ecx+0x04]
    skip_inc:
        mov  eax, [ecx+0x04]
        push ebx
        mov  bl, byte ptr [ecx+0x90]
        dec  eax
        mov  dword ptr [ecx+0x04], eax
        jne  no_release
        mov  eax, [ecx]
        call dword ptr [eax+0x04]
    no_release:
        mov  al, bl
        pop  ebx
        ret
    }
}

// Refcounted sub-object layout:
//   +0x00 vtable ptr, +0x04 refcount, +0x90 analogue-loading byte
static int g_released = 0;
static void __stdcall FakeRelease() { g_released = 1; }

// vtable: slot0 unused, slot1 (+4) = release
static void* g_vtable[4];

// sub-object buffer (needs >=0x91 bytes)
static unsigned char g_sub[0x100];

// outer object: +0x0c points at sub-object
static void* g_outer[8];

static unsigned char call_it(void* self)
{
    unsigned char r;
    __asm
    {
        mov ecx, self
        call IsAnalogueLoading
        mov r, al
    }
    return r;
}

int main()
{
    g_vtable[1] = (void*)&FakeRelease;

    // configure sub-object
    *(void**)(g_sub + 0x00) = g_vtable;
    *(int*)   (g_sub + 0x04) = 5;          // refcount high -> no release
    g_sub[0x90] = 0x42;                     // the returned byte

    g_outer[0x0c/4] = g_sub;                // this+0x0c -> sub-object

    unsigned char v = call_it(g_outer);
    if (v != 0x42) { printf("FAIL value %u\n", (unsigned)v); return 1; }
    if (*(int*)(g_sub+0x04) != 5) { printf("FAIL refcount %d\n", *(int*)(g_sub+0x04)); return 1; }
    if (g_released != 0) { printf("FAIL released early\n"); return 1; }

    // now drive the release path: refcount==1 so after inc(2)/dec(1)... wait
    // inc ->6? no: reset to 1 so inc->2, dec->1, jne taken, no release.
    // Use refcount 0 so inc(1)... test-je: ptr nonnull so inc runs ->1, dec->0, release.
    *(int*)(g_sub + 0x04) = 0;
    g_released = 0;
    v = call_it(g_outer);
    if (v != 0x42) { printf("FAIL value2 %u\n", (unsigned)v); return 1; }
    if (g_released != 1) { printf("FAIL not released\n"); return 1; }

    printf("OK_0x005da8f0\n");
    return 0;
}