#include <stdio.h>
#include <string.h>

static int g_calls;

/* Masked subobject copy helpers. source_cpp references them as __fastcall(void*,void*)
 * so they carry the @8-decorated names @Uninit_copy_uint_047c290_CopyA@8 /
 * @...CopyB@8. The routine invokes them thiscall-style: this=ecx=&dst.sub and ONE
 * stack arg=&src.sub, then `ret 4`. A naked body reproduces that exactly (a normal
 * __fastcall body would read arg2 from edx and `ret 8`, corrupting the caller's
 * stack). Each stub copies one dword *dst = *src and bumps the call counter. */
extern "C" __declspec(naked) void __fastcall
Uninit_copy_uint_047c290_CopyA(void* dst_ecx, void* src_stack)
{
    __asm {
        mov  eax, [esp+4]
        mov  eax, [eax]
        mov  [ecx], eax
        inc  g_calls
        ret  4
    }
}

extern "C" __declspec(naked) void __fastcall
Uninit_copy_uint_047c290_CopyB(void* dst_ecx, void* src_stack)
{
    __asm {
        mov  eax, [esp+4]
        mov  eax, [eax]
        mov  [ecx], eax
        inc  g_calls
        ret  4
    }
}

/* Target lives in source.obj; prototype matches @Uninit_copy_uint_047c290@12. */
extern "C" void* __fastcall Uninit_copy_uint_047c290(void*, void*, void*);

/* Wrapper: ecx=first, edx=last; push masked-alloc then dest. The target does
 * `ret 8` (pops both stack dwords) and returns the advanced dest pointer in eax. */
extern "C" __declspec(naked) void* __cdecl
call_uc(void* first, void* last, void* dest)
{
    __asm {
        mov  ecx, [esp+4]
        mov  edx, [esp+8]
        push 0
        push dword ptr [esp+16]
        call Uninit_copy_uint_047c290
        ret
    }
}

struct Elem {
    unsigned long a;    /* +0x00 raw   */
    unsigned long b;    /* +0x04 sub A */
    unsigned long c;    /* +0x08 raw   */
    unsigned long d;    /* +0x0c sub B */
    unsigned long p0;   /* +0x10 pad   */
    unsigned long p1;   /* +0x14 pad   */
};

int main()
{
    int failures = 0;
    int i;
    static Elem SRC[3];
    static Elem DST[4];
    for (i = 0; i < 3; ++i) {
        SRC[i].a  = 0xA0000000u + i*6 + 0;
        SRC[i].b  = 0xA0000000u + i*6 + 1;
        SRC[i].c  = 0xA0000000u + i*6 + 2;
        SRC[i].d  = 0xA0000000u + i*6 + 3;
        SRC[i].p0 = 0xA0000000u + i*6 + 4;
        SRC[i].p1 = 0xA0000000u + i*6 + 5;
    }
    memset(DST, 0xEE, sizeof(DST));

    g_calls = 0;
    void* ret = call_uc(&SRC[0], &SRC[3], &DST[0]);

    if (g_calls != 6) { printf("call-count failed: %d (want 6)\n", g_calls); ++failures; }
    for (i = 0; i < 3; ++i) {
        if (DST[i].a != SRC[i].a) { printf("a mismatch %d\n", i); ++failures; }
        if (DST[i].b != SRC[i].b) { printf("b mismatch %d\n", i); ++failures; }
        if (DST[i].c != SRC[i].c) { printf("c mismatch %d\n", i); ++failures; }
        if (DST[i].d != SRC[i].d) { printf("d mismatch %d\n", i); ++failures; }
    }
    if (DST[3].a != 0xEEEEEEEEu) { printf("overrun into trailing element\n"); ++failures; }
    if (ret != (void*)&DST[3]) { printf("ret %p want %p\n", ret, (void*)&DST[3]); ++failures; }

    g_calls = 0;
    void* ret2 = call_uc(&SRC[0], &SRC[0], &DST[0]);
    if (g_calls != 0 || ret2 != (void*)&DST[0]) { printf("empty-range fail calls=%d ret=%p\n", g_calls, ret2); ++failures; }

    if (failures) { printf("UNINIT_COPY_UINT_C_0047C290_TEST FAIL count=%d\n", failures); return 1; }
    printf("UNINIT_COPY_UINT_C_0047C290_TEST PASS\n");
    return 0;
}