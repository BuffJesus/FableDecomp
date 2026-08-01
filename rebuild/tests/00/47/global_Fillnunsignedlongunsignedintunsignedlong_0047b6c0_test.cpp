/* Behavior test for _Fill_n_ulong_ptr_uint_ulong_0047b6c0 (retail 0x0047b6c0).
 * Self-contained; compiles under cl /Od, links, runs.
 *
 * The retail function fills `count` copies of a 12-byte value into a destination
 * buffer, copy-constructing each of the three 4-byte sub-objects via the masked
 * __fastcall helper Sub_520060(this=&dest.sub, arg=&src.sub). We supply a stub
 * Sub_520060 that copies one 4-byte word (dword ptr [ecx] = *[esp+4]) so we can
 * observe the fill and the returned advanced pointer.
 */

#include <stdio.h>
#include <string.h>

/* Stub for the masked __fastcall subobject copier: *this = *src (one dword). */
extern "C" __declspec(naked) void Sub_520060(void)
{
    __asm
    {
        mov  eax, [esp + 4]   ; src pointer (single stack arg)
        mov  eax, [eax]       ; *src
        mov  [ecx], eax       ; *this = *src
        ret  4                ; __fastcall pops the single stack arg
    }
}

extern "C" void __fastcall
_Fill_n_ulong_ptr_uint_ulong_0047b6c0(
    void* destination, unsigned int count, const void* value);

struct Elem { unsigned long a, b, c; };

int main(void)
{
    Elem val;
    val.a = 0x11111111u;
    val.b = 0x22222222u;
    val.c = 0x33333333u;

    const unsigned int N = 4;
    Elem dst[6];
    memset(dst, 0xEE, sizeof(dst));

    void* ret = 0;
    /* __fastcall: ecx=destination, edx=count, stack arg=value ptr. */
    ret = (void*)0;
    typedef void (__fastcall *FillFn)(void*, unsigned int, const void*);
    FillFn fn = _Fill_n_ulong_ptr_uint_ulong_0047b6c0;
    /* wrap to capture eax return (void proto), read via inline asm */
    void* got;
    {
        void* d = dst;
        unsigned int cnt = N;
        const void* v = &val;
        __asm
        {
            mov  ecx, d
            mov  edx, cnt
            mov  eax, v
            push eax
            call fn
            mov  got, eax
        }
    }
    (void)ret;

    int ok = 1;
    for (unsigned int i = 0; i < N; ++i)
    {
        if (dst[i].a != val.a || dst[i].b != val.b || dst[i].c != val.c)
            ok = 0;
    }
    /* element N must be untouched (still 0xEEEEEEEE) */
    if (dst[N].a != 0xEEEEEEEEu)
        ok = 0;
    /* returned pointer must equal dst + N (advanced by count*12 bytes) */
    if (got != (void*)(dst + N))
        ok = 0;

    if (ok)
        printf("FILL_N_ULONG_TRIPLE_TEST PASS\n");
    else
        printf("FILL_N_ULONG_TRIPLE_TEST FAIL\n");
    return ok ? 0 : 1;
}