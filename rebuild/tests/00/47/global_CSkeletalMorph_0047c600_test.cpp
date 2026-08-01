#include <cstdio>

// --- masked copy helpers: real sub-object copy-assign. For the behaviour test
// we model each sub-object as a single u32 and copy it. __fastcall/naked so the
// register/stack ABI matches the caller emitted by the function under test
// (this in ecx, source address pushed on the stack, callee pops it via ret 4). ---
extern "C" __declspec(naked) void __fastcall CSkeletalMorph_CopyMemberA(void* /*dst ecx*/, void* /*src stack*/)
{
    __asm {
        mov eax, dword ptr [esp + 4]   // src
        mov eax, dword ptr [eax]       // *src
        mov dword ptr [ecx], eax       // *dst = *src
        ret 4
    }
}
extern "C" __declspec(naked) void __fastcall CSkeletalMorph_CopyMemberB(void* /*dst ecx*/, void* /*src stack*/)
{
    __asm {
        mov eax, dword ptr [esp + 4]
        mov eax, dword ptr [eax]
        mov dword ptr [ecx], eax
        ret 4
    }
}

extern "C" __declspec(naked) void* __fastcall
Copy_impl_CSkeletalMorph_0047c600(void* /*ecx _First*/, void* /*edx _Last*/, void* /*_Dest*/)
{
    __asm
    {
        push esi
        mov  esi, ecx
        sub  edx, esi
        mov  eax, 2AAAAAABh
        imul edx
        sar  edx, 2
        mov  eax, edx
        shr  eax, 1Fh
        add  eax, edx
        test eax, eax
        jle  empty

        push ebx
        push edi
        mov  edi, dword ptr [esp + 10h]
        mov  ebx, eax
    loop_top:
        mov  eax, dword ptr [esi]
        lea  ecx, dword ptr [esi + 4]
        push ecx
        lea  ecx, dword ptr [edi + 4]
        mov  dword ptr [edi], eax
        call CSkeletalMorph_CopyMemberA
        mov  edx, dword ptr [esi + 8]
        lea  eax, dword ptr [esi + 0Ch]
        push eax
        lea  ecx, dword ptr [edi + 0Ch]
        mov  dword ptr [edi + 8], edx
        call CSkeletalMorph_CopyMemberB
        add  esi, 18h
        add  edi, 18h
        dec  ebx
        jne  loop_top

        mov  eax, edi
        pop  edi
        pop  ebx
        pop  esi
        ret  0Ch
    empty:
        mov  eax, dword ptr [esp + 8]
        pop  esi
        ret  0Ch
    }
}

// 24-byte element: two {u32; u32} pairs + 8 bytes trailing pad.
struct CSkeletalMorph { unsigned int a0, s0, a8, sc, p10, p14; };

// __fastcall wrapper: _First/_Last in ecx/edx, _Dest on the stack.
static void* call_copy(CSkeletalMorph* first, CSkeletalMorph* last, CSkeletalMorph* dest)
{
    void* r;
    __asm {
        mov ecx, first
        mov edx, last
        push dest
        call Copy_impl_CSkeletalMorph_0047c600
        mov r, eax
    }
    return r;
}

int main()
{
    const int N = 3;
    CSkeletalMorph src[N];
    CSkeletalMorph dst[N];
    int i;
    for (i = 0; i < N; ++i) {
        src[i].a0 = 1000 + i; src[i].s0 = 2000 + i;
        src[i].a8 = 3000 + i; src[i].sc = 4000 + i;
        src[i].p10 = 5000 + i; src[i].p14 = 6000 + i;
        dst[i].a0 = dst[i].s0 = dst[i].a8 = dst[i].sc = dst[i].p10 = dst[i].p14 = 0xFFFFFFFFu;
    }

    void* end = call_copy(&src[0], &src[N], &dst[0]);
    if (end != (void*)&dst[N]) { std::printf("FAIL end ptr\n"); return 1; }

    for (i = 0; i < N; ++i) {
        if (dst[i].a0 != src[i].a0) { std::printf("FAIL a0 %d\n", i); return 1; }
        if (dst[i].s0 != src[i].s0) { std::printf("FAIL s0 %d\n", i); return 1; }
        if (dst[i].a8 != src[i].a8) { std::printf("FAIL a8 %d\n", i); return 1; }
        if (dst[i].sc != src[i].sc) { std::printf("FAIL sc %d\n", i); return 1; }
    }

    // Empty-range branch returns _Dest unchanged.
    void* e2 = call_copy(&src[0], &src[0], &dst[0]);
    if (e2 != (void*)&dst[0]) { std::printf("FAIL empty ptr\n"); return 1; }

    std::printf("COPY_IMPL_CSKELETALMORPH_0047C600_TEST PASS\n");
    return 0;
}