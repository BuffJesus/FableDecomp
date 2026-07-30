#include <stdio.h>

__declspec(naked) float __fastcall GetAnimationSpeedValue(void* ecx)
{
    (void)ecx;
    __asm {
        push ebp
        mov  ebp, esp
        push ecx
        mov  ecx, dword ptr [ecx+0x68]
        test ecx, ecx
        je   skip_add
        inc  dword ptr [ecx+0x4]
    skip_add:
        mov  eax, dword ptr [ecx+0x3c]
        dec  dword ptr [ecx+0x4]
        mov  dword ptr [ebp-4], eax
        jne  no_rel
        mov  eax, dword ptr [ecx]
        call dword ptr [eax+4]
    no_rel:
        fld  dword ptr [ebp-4]
        leave
        ret
    }
}

// Inner ref-counted object: vptr @+0, refcount @+4, float @+0x3c
struct Inner {
    void*         vptr;   // +0x00
    long          refcount; // +0x04
    char          pad[0x3c - 0x08]; // +0x08 .. +0x3b
    float         value;  // +0x3c
};

// Outer object with the handle at +0x68
struct Outer {
    char  pad[0x68];      // +0x00 .. +0x67
    Inner* handle;        // +0x68
};

static int g_released = 0;
void __fastcall FakeRelease(void* ecx) { (void)ecx; g_released = 1; }

int main()
{
    // vtable with slot 1 (+0x4) = release
    void* vt[4];
    vt[0] = 0;
    vt[1] = (void*)&FakeRelease;
    vt[2] = 0;
    vt[3] = 0;

    Inner inner;
    inner.vptr = (void*)vt;
    inner.refcount = 5;      // inc->6, dec->5 (nonzero) => no release, returns value
    inner.value = 1.75f;

    Outer outer;
    for (int i = 0; i < (int)sizeof(outer.pad); ++i) outer.pad[i] = 0;
    outer.handle = &inner;

    float r;
    void* self = &outer;
    __asm {
        mov ecx, self
        call GetAnimationSpeedValue
        fstp r
    }

    if (r != 1.75f) { printf("FAIL value r=%f\n", r); return 1; }
    if (inner.refcount != 5) { printf("FAIL refcount=%ld\n", inner.refcount); return 1; }
    if (g_released != 0) { printf("FAIL released unexpectedly\n"); return 1; }

    // Second case: refcount 1 => inc->2, dec->1 (nonzero) still no release; verify field read again
    inner.refcount = 1;
    inner.value = -3.5f;
    __asm {
        mov ecx, self
        call GetAnimationSpeedValue
        fstp r
    }
    if (r != -3.5f) { printf("FAIL value2 r=%f\n", r); return 1; }

    printf("OK_0x005bd404 value=%f\n", 1.75);
    return 0;
}