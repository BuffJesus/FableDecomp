#include <stdio.h>
#include <string.h>

static int g_calls;

/* Masked subobject copy helper. Source references it as __fastcall(...) so it
 * must carry the @8-decorated name. The real target (0x5200b0, __fastcall) takes
 * this=ecx=&dst and ONE stack arg=&src, and returns with ret 4. We reproduce
 * that exactly with a naked body (a normal __fastcall body would read arg2 from
 * edx and ret 8, which does not match how the routine invokes it). */
extern "C" __declspec(naked) void __fastcall
Uninit_copy_uint_047b670_CopyCtor(void* dst_ecx, void* src_stack)
{
    __asm {
        mov  eax, [esp+4]   ; src (single stack arg the routine pushed)
        mov  eax, [eax]     ; *src
        mov  [ecx], eax     ; *dst = *src   (dst = this = ecx)
        inc  g_calls
        ret  4              ; pop the single stack arg
    }
}

extern "C" void* __fastcall Uninit_copy_uint_047b670(void*, void*, void*);

/* __cdecl naked wrapper: ecx=first, edx=last; push masked-alloc then dest; the
 * target does ret 8 (pops both) and returns the advanced dest pointer in eax. */
extern "C" __declspec(naked) void* __cdecl
call_uc(void* first, void* last, void* dest)
{
    __asm {
        mov  ecx, [esp+4]         ; first
        mov  edx, [esp+8]         ; last
        push 0                    ; masked allocator reference
        push dword ptr [esp+16]   ; dest (esp shifted +4 by the push above)
        call Uninit_copy_uint_047b670
        ret
    }
}

struct Elem { unsigned long a, b, c; };
int main()
{
    int failures = 0;
    static Elem SRC[3];
    static Elem DST[4];
    for (int i = 0; i < 3; ++i) {
        SRC[i].a = 0xA0000000u + i*3 + 0;
        SRC[i].b = 0xA0000000u + i*3 + 1;
        SRC[i].c = 0xA0000000u + i*3 + 2;
    }
    memset(DST, 0xEE, sizeof(DST));

    g_calls = 0;
    void* ret = call_uc(&SRC[0], &SRC[3], &DST[0]);

    if (g_calls != 9) { printf("call-count failed: %d (want 9)\n", g_calls); ++failures; }
    if (memcmp(DST, SRC, sizeof(SRC)) != 0) { printf("payload mismatch\n"); ++failures; }
    if (DST[3].a != 0xEEEEEEEEu) { printf("overrun into trailing element\n"); ++failures; }
    if (ret != (void*)&DST[3]) { printf("ret %p want %p\n", ret, (void*)&DST[3]); ++failures; }

    g_calls = 0;
    void* ret2 = call_uc(&SRC[0], &SRC[0], &DST[0]);
    if (g_calls != 0 || ret2 != (void*)&DST[0]) { printf("empty-range fail calls=%d ret=%p\n", g_calls, ret2); ++failures; }

    if (failures) { printf("UNINIT_COPY_UINT_0047B670_TEST FAIL count=%d\n", failures); return 1; }
    printf("UNINIT_COPY_UINT_0047B670_TEST PASS\n");
    return 0;
}