#include <cstdio>
#include "rebuild_abi.h"

// Standalone harness for __ArrayUnwind (0x00BFEE7A).
// The retail __EH_prolog / __EH_epilog CRT internals are relocation-masked
// in parity; here we substitute frame-establishing stand-ins so the naked
// thunk executes deterministically, and observe the destructor-callback
// side effects through sentinel globals. Only the main-body path (the
// __try loop) is exercised; the ArrayUnwindFilter tail is unreachable
// without CRT-driven exception dispatch and is not exercised by this test
// (see source comment).
//
// Stack when EH_prolog_00bfee7a is entered (stdcall call site already did:
// push 0x14 ; push funcinfo ; call EH_prolog):
//   [esp+0]  = our own return address (back into __ArrayUnwind, right
//              after the 'call EH_prolog_00bfee7a' instruction)
//   [esp+4]  = 0x14
//   [esp+8]  = funcinfo ptr
//   [esp+0xc]= __ArrayUnwind's own return address (into main())
//   [esp+0x10] = arg_base
//   [esp+0x14] = arg_size
//   [esp+0x18] = arg_count
//   [esp+0x1c] = arg_dtor
//
// The real _EH_prolog sets ebp so that ebp+8 == arg_base's address, i.e.
// ebp == (esp at entry) + 8. We save that ebp so EH_epilog can later
// restore esp to point exactly at __ArrayUnwind's own return-address slot:
// that address is (saved ebp) + 4.

extern "C" char g_funcinfo_00bfee7a[8];
char g_funcinfo_00bfee7a[8] = {0};

static int g_prolog_hit = 0;
static int g_epilog_hit = 0;
static int g_dtor_calls = 0;
static void* g_dtor_this[8];

static void* g_saved_frame_esp_at_prolog_entry = 0;
static void* g_saved_caller_ebp = 0;

extern "C" __declspec(naked) void FABLE_CDECL EH_prolog_00bfee7a(void)
{
    __asm
    {
        mov   g_saved_caller_ebp, ebp
        mov   eax, esp
        mov   g_saved_frame_esp_at_prolog_entry, eax
        inc   dword ptr [g_prolog_hit]
        mov   ebp, eax
        add   ebp, 8
        ret
    }
}

extern "C" __declspec(naked) void FABLE_CDECL EH_epilog_00bfee7a(void)
{
    __asm
    {
        inc   dword ptr [g_epilog_hit]
        mov   eax, g_saved_frame_esp_at_prolog_entry
        add   eax, 0x0c
        mov   esp, eax
        mov   ebp, g_saved_caller_ebp
        ret
    }
}

extern "C" __declspec(naked) void FABLE_CDECL UnwindContinue_00bfee7a(void)
{
    __asm
    {
        ret
    }
}

extern "C" __declspec(naked) void FABLE_CDECL fake_dtor_00bfee7a(void)
{
    __asm
    {
        mov   eax, [g_dtor_calls]
        cmp   eax, 8
        jge   noStore
        lea   edx, [g_dtor_this]
        mov   [edx+eax*4], ecx
    noStore:
        inc   dword ptr [g_dtor_calls]
        ret
    }
}

extern "C" void FABLE_STDCALL ArrayUnwind_00bfee7a(void);

__declspec(naked) void FABLE_STDCALL ArrayUnwind_00bfee7a(void)
{
    __asm
    {
        push  0x14
        push  offset g_funcinfo_00bfee7a
        call  EH_prolog_00bfee7a
        and   dword ptr [ebp-4], 0
    loop_top:
        dec   dword ptr [ebp+0x10]
        js    loop_end
        mov   ecx, dword ptr [ebp+8]
        sub   ecx, dword ptr [ebp+0xc]
        mov   dword ptr [ebp+8], ecx
        call  dword ptr [ebp+0x14]
        jmp   loop_top
    loop_end:
        or    dword ptr [ebp-4], 0xffffffff
        call  EH_epilog_00bfee7a
        ret   0x10

        mov   eax, dword ptr [ebp-0x14]
        mov   dword ptr [ebp-0x1c], eax
        mov   eax, dword ptr [ebp-0x1c]
        mov   eax, dword ptr [eax]
        mov   dword ptr [ebp-0x20], eax
        mov   eax, dword ptr [ebp-0x20]
        cmp   dword ptr [eax], 0e06d7363h
        je    is_cxx_eh
        mov   dword ptr [ebp-0x24], 0
        mov   eax, dword ptr [ebp-0x24]
        ret
    resume_unwind:
        mov   esp, dword ptr [ebp-0x18]
        jmp   loop_end
    is_cxx_eh:
        call  UnwindContinue_00bfee7a
    }
}

int main(void)
{
    // base=0x1000 (fake), size=4, count=3, dtor=fake_dtor.
    // base starts unmodified at 0x1000; each iteration does base -= size
    // BEFORE calling dtor(base), so the callback sees 0xFFC, 0xFF8, 0xFF4
    // in that order.
    typedef void (FABLE_CDECL *DtorFn)(void);
    void* arg_base = (void*)0x1000;
    unsigned arg_size = 4;
    int arg_count = 3;
    DtorFn arg_dtor = fake_dtor_00bfee7a;

    __asm
    {
        push  arg_dtor
        push  arg_count
        push  arg_size
        push  arg_base
        call  ArrayUnwind_00bfee7a
    }

    int ok = (g_prolog_hit == 1) && (g_epilog_hit == 1) &&
              (g_dtor_calls == 3) &&
              (g_dtor_this[0] == (void*)0xFFC) &&
              (g_dtor_this[1] == (void*)0xFF8) &&
              (g_dtor_this[2] == (void*)0xFF4);

    if (ok)
        printf("PASS_00BFEE7A_OK\n");
    else
        printf("FAIL_00BFEE7A p=%d e=%d n=%d t0=%p t1=%p t2=%p\n",
               g_prolog_hit, g_epilog_hit, g_dtor_calls,
               g_dtor_this[0], g_dtor_this[1], g_dtor_this[2]);
    return 0;
}