#include <cstdio>

struct Member { unsigned int m; };

static int g_copy_calls = 0;

// Masked copy helper used by the target for the +4 sub-member.
//
// It must mangle EXACTLY as @Uninit_copy_uint_047b5f0_CopyCtor@8 to resolve the
// undefined symbol source_cpp references, so we declare it __fastcall (two
// params -> @8). BUT the retail caller invokes it __fastcall-style: the object
// (this = &dst.b) arrives in ecx and the source (&src.b) is pushed as a SINGLE
// STACK argument, and the callee cleans that dword (ret 4). A plain C
// __fastcall(void*,void*) body would take the second operand from EDX and
// `ret 0`, leaving the pushed dword on the stack every iteration -> esp/esi
// drift -> access violation. So we implement it NAKED and read [esp+4].
extern "C" __declspec(naked) void __fastcall
Uninit_copy_uint_047b5f0_CopyCtor(void* /*dst ecx*/, void* /*src stack*/)
{
    __asm
    {
        mov eax, [esp + 4]     ; src (the single stack argument)
        mov eax, [eax]         ; *src (the uint sub-member value)
        mov [ecx], eax         ; *this = *src
        inc dword ptr [g_copy_calls]
        ret 4                  ; pop the one stack argument
    }
}

// The target: __fastcall(ecx=First, edx=Last, stack=Dest). Its naked body ends
// in `ret 8`, i.e. the real retail caller pushes TWO stack dwords (Dest plus the
// masked allocator ref). Keep the 3-arg prototype so the C++ decoration stays
// @Uninit_copy_uint_047b5f0@12 (matching source.obj), then INVOKE it via inline
// asm that pushes both dwords so the `ret 8` cleanup balances the stack. A plain
// C call pushes only Dest (one dword) and `ret 8` corrupts the caller stack.
extern "C" void* __fastcall
Uninit_copy_uint_047b5f0(void* First, void* Last, void* Dest);

struct Elem {
    unsigned int a;   // +0
    Member       b;   // +4
    char         c;   // +8
};                    // sizeof == 12

// Invoke with the exact retail stack shape and capture the eax return (Dest').
static void* call_target(void* First, void* Last, void* Dest)
{
    void* result = 0;
    void* alloc  = 0;   // masked allocator ref: pushed, never dereferenced
    __asm
    {
        mov  eax, alloc
        push eax
        mov  eax, Dest
        push eax
        mov  ecx, First
        mov  edx, Last
        call Uninit_copy_uint_047b5f0
        mov  result, eax
    }
    return result;
}

int main()
{
    Elem src[3];
    Elem dst[3];
    unsigned int i;
    for (i = 0; i < 3; ++i) {
        src[i].a = 100 + i;
        src[i].b.m = 200 + i;
        src[i].c = (char)(1 + i);
        dst[i].a = 0; dst[i].b.m = 0; dst[i].c = 0;
    }

    g_copy_calls = 0;
    void* end = call_target(&src[0], &src[3], &dst[0]);

    if (end != &dst[3]) { std::printf("FAIL end\n"); return 1; }
    if (g_copy_calls != 3) { std::printf("FAIL copycalls %d\n", g_copy_calls); return 1; }
    for (i = 0; i < 3; ++i) {
        if (dst[i].a != 100 + i) { std::printf("FAIL a %u\n", i); return 1; }
        if (dst[i].b.m != 200 + i) { std::printf("FAIL b %u\n", i); return 1; }
        if (dst[i].c != (char)(1 + i)) { std::printf("FAIL c %u\n", i); return 1; }
    }

    // empty range: First == Last -> no copies, returns Dest unchanged
    g_copy_calls = 0;
    void* r = call_target(&src[0], &src[0], &dst[0]);
    if (r != &dst[0]) { std::printf("FAIL empty end\n"); return 1; }
    if (g_copy_calls != 0) { std::printf("FAIL empty copies\n"); return 1; }

    std::printf("UNINIT_COPY_UINT_A_0047B5F0_TEST PASS\n");
    return 0;
}