#include <stdio.h>
#include <string.h>

// Behavior test for _Uninit_copy @ 0x0047b710.
//
// The routine copy-constructs a run of 12-byte elements (three 4-byte
// sub-members at +0/+4/+8) from [first,last) into dest, one sub-member at a
// time via a masked __fastcall-style copy helper, then returns the advanced
// dest pointer in eax.
//
// Two calling-convention details make a plain C call impossible here:
//
//  1. The target is __fastcall (ecx=first, edx=last) with the dest pointer on
//     the stack, and it ends with `ret 8` -- it pops TWO stack dwords (the dest
//     pointer PLUS the masked allocator reference the retail caller also
//     pushes).  A C __fastcall call with one stack arg pushes only ONE dword,
//     so the callee over-pops by 4 and corrupts esp.  We invoke it via inline
//     asm, pushing a dummy allocator dword and the dest pointer, capturing eax.
//
//  2. The masked copy helper is called with `this` in ecx and the src pointer
//     pushed as ONE stack arg, then it does `ret 4`.  A C __fastcall(void*,
//     void*) stub would take BOTH args in registers (ret 0) and leave the
//     pushed arg on the stack -> corruption.  So the stub is a naked __fastcall
//     (for the @8-decorated symbol the source references) that reads src from
//     [esp+4], writes *this = *src, and does `ret 4`.

static int g_copyCalls = 0;

extern "C" __declspec(naked) void __fastcall
Uninit_copy_uint_047b710_CopyCtor(void* /*dst_ecx*/, void* /*src_stack*/)
{
    __asm
    {
        mov  eax, [esp + 4]     ; src sub-member pointer (single stack arg)
        mov  eax, [eax]         ; *src
        mov  [ecx], eax         ; *this = *src
        inc  dword ptr [g_copyCalls]
        ret  4                  ; pop the one stack arg
    }
}

extern "C" void* __fastcall
Uninit_copy_uint_047b710(void* first_ecx, void* last_edx, void* dest);

struct Elem { unsigned int a, b, c; };

// Invoke the target with the correct stack shape for `ret 8`:
// push [dummy allocator][dest], ecx=first, edx=last, return eax.
static void* call_uninit_copy(void* first, void* last, void* dest)
{
    void* got;
    __asm
    {
        mov  ecx, first
        mov  edx, last
        mov  eax, dest
        push 0
        push eax
        call Uninit_copy_uint_047b710
        mov  got, eax
    }
    return got;
}

int main(void)
{
    Elem src[3];
    src[0].a = 0x11111111u; src[0].b = 0x22222222u; src[0].c = 0x33333333u;
    src[1].a = 0x44444444u; src[1].b = 0x55555555u; src[1].c = 0x66666666u;
    src[2].a = 0x77777777u; src[2].b = 0x88888888u; src[2].c = 0x99999999u;

    Elem dst[3];
    memset(dst, 0, sizeof(dst));

    g_copyCalls = 0;
    void* ret = call_uninit_copy(&src[0], &src[3], &dst[0]);

    // 3 elements x 3 sub-members = 9 copy-helper invocations.
    int okCalls = (g_copyCalls == 9);
    // Returned dest advanced by 3 elements.
    int okRet = (ret == (void*)&dst[3]);
    // Contents copied verbatim.
    int okData = (memcmp(src, dst, sizeof(src)) == 0);

    // Empty range: no copies, returns dest unchanged.
    Elem e0;
    g_copyCalls = 0;
    void* ret0 = call_uninit_copy(&src[0], &src[0], &e0);
    int okEmpty = (g_copyCalls == 0) && (ret0 == (void*)&e0);

    if (okCalls && okRet && okData && okEmpty)
        printf("UNINIT_COPY_UINT_047B710_TEST PASS\n");
    else
        printf("UNINIT_COPY_UINT_047B710_TEST FAIL calls=%d ret=%d data=%d empty=%d\n",
               okCalls, okRet, okData, okEmpty);
    return 0;
}