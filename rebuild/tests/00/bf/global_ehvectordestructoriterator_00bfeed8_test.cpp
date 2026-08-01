#include <cstdio>
#include "rebuild_abi.h"

// Standalone harness for eh_vector_destructor_iterator (0x00BFEED8).
// The retail __EH_prolog3 / __EH_epilog3 / _local_unwind2 CRT internals are
// relocation-masked in parity; here we substitute frame-establishing
// stand-ins so the naked thunk executes deterministically, and observe the
// destructor-callback side effects through sentinel globals.
//
// Call-site stack layout right when our EH_prolog3 stub is entered (stdcall,
// callee already did: push 0x0c ; push scopetable ; call prolog3):
//   [esp+0]  = return address (back into eh_vector_..., right after the call)
//   [esp+4]  = 0x0c
//   [esp+8]  = scopetable ptr
//   [esp+0xc]= return address of eh_vector_... itself (into main/test)
//   [esp+0x10] = base   (arg1)
//   [esp+0x14] = size   (arg2)
//   [esp+0x18] = count  (arg3)
//   [esp+0x1c] = dtor   (arg4)
//
// The real _EH_prolog3 sets ebp so that ebp+8 == arg1's address, i.e.
// ebp == [esp+0xc] (the eh_vector_... return-address slot) ... actually
// ebp must equal (address of eh_vector_...'s own return address), since
// that is what "[ebp+8]" resolves to arg1 with the standard __stdcall
// frame (ret-addr at ebp+0, wait -- standard frame has saved-ebp at
// ebp+0, ret-addr at ebp+4, arg1 at ebp+8). So ebp must point at a slot
// that itself holds a saved ebp value at ebp+0 and the return address at
// ebp+4. We synthesize that by writing a fabricated saved-ebp/retaddr
// pair directly below the true retaddr-of-eh_vector_... slot and pointing
// ebp there, without disturbing the real args above it.

extern "C" char g_scopetable_00bfeed8[8];
char g_scopetable_00bfeed8[8] = {0};

static int g_prolog_hit = 0;
static int g_epilog_hit = 0;
static int g_unwind_hit = 0;
static int g_dtor_calls = 0;
static void* g_dtor_this[8];

// On entry to EH_prolog3_00bfeed8:
//   [esp+0]    = our own return address (back into eh_vector_..., right
//                after the 'call EH_prolog3_00bfeed8' instruction)
//   [esp+4]    = 0x0c                 (pushed by the callee)
//   [esp+8]    = scopetable ptr       (pushed by the callee)
//   [esp+0xc]  = eh_vector_...'s own return address (into main())
//   [esp+0x10] = arg_base
//   [esp+0x14] = arg_size
//   [esp+0x18] = arg_count
//   [esp+0x1c] = arg_dtor
//
// We must return with ebp set so that ebp+8 == address of arg_base, i.e.
// ebp == (current esp) + 8. We save that ebp in a static slot (rather than
// building a fabricated stack frame) so EH_epilog3 can later restore esp to
// point exactly at eh_vector_...'s own return-address slot: that address is
// (saved ebp) + 4.
static void* g_saved_frame_esp_at_prolog_entry = 0;
static void* g_saved_caller_ebp = 0;

extern "C" __declspec(naked) void FABLE_CDECL EH_prolog3_00bfeed8(void)
{
    __asm
    {
        mov   g_saved_caller_ebp, ebp       ; preserve caller's ebp (main's frame)
        mov   eax, esp                      ; eax = esp at entry (points at our retaddr)
        mov   g_saved_frame_esp_at_prolog_entry, eax
        inc   dword ptr [g_prolog_hit]
        mov   ebp, eax
        add   ebp, 8                        ; ebp+8 = arg_base's address
        ret
    }
}

extern "C" __declspec(naked) void FABLE_CDECL local_unwind2_00bfeed8(void)
{
    __asm
    {
        inc dword ptr [g_unwind_hit]
        ret
    }
}

extern "C" __declspec(naked) void FABLE_CDECL EH_epilog3_00bfeed8(void)
{
    __asm
    {
        inc   dword ptr [g_epilog_hit]
        ; Restore esp to point exactly at eh_vector_...'s own return-address
        ; slot ([esp_at_prolog_entry + 0xc]) so the callee's trailing
        ; 'ret 0x10' pops that address and cleans the 4 stdcall args.
        ; Also restore the caller's ebp (the real _EH_epilog3 pops the saved
        ; ebp as part of SEH-frame teardown).
        mov   eax, g_saved_frame_esp_at_prolog_entry
        add   eax, 0x0c
        mov   esp, eax
        mov   ebp, g_saved_caller_ebp
        ret
    }
}

extern "C" __declspec(naked) void FABLE_CDECL fake_dtor_00bfeed8(void)
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

extern "C" void FABLE_STDCALL eh_vector_destructor_iterator(void);

__declspec(naked) void FABLE_STDCALL eh_vector_destructor_iterator(void)
{
    __asm
    {
        push  0x0c
        push  offset g_scopetable_00bfeed8
        call  EH_prolog3_00bfeed8
        and   dword ptr [ebp-0x1c], 0
        mov   esi, [ebp+0x0c]
        mov   eax, esi
        imul  eax, [ebp+0x10]
        add   [ebp+0x08], eax
        and   dword ptr [ebp-0x04], 0
    loop_top:
        dec   dword ptr [ebp+0x10]
        js    loop_end
        sub   [ebp+0x08], esi
        mov   ecx, [ebp+0x08]
        call  dword ptr [ebp+0x14]
        jmp   loop_top
    loop_end:
        mov   dword ptr [ebp-0x1c], 1
        or    dword ptr [ebp-0x04], 0xffffffff
        call  local_unwind2_00bfeed8
        call  EH_epilog3_00bfeed8
        ret   0x10
    }
}

int main(void)
{
    // base=0x1000 (fake), size=4, count=3, dtor=fake_dtor.
    // p starts at base+size*count = 0x100c; each iteration does p -= size
    // BEFORE calling dtor(p), so the callback sees 0x1008, 0x1004, 0x1000
    // in that order (never sees 0x100c itself).
    typedef void (FABLE_CDECL *DtorFn)(void);
    void* arg_base = (void*)0x1000;
    unsigned arg_size = 4;
    int arg_count = 3;
    DtorFn arg_dtor = fake_dtor_00bfeed8;

    __asm
    {
        push  arg_dtor
        push  arg_count
        push  arg_size
        push  arg_base
        call  eh_vector_destructor_iterator
    }

    int ok = (g_prolog_hit == 1) && (g_epilog_hit == 1) && (g_unwind_hit == 1) &&
              (g_dtor_calls == 3) &&
              (g_dtor_this[0] == (void*)0x1008) &&
              (g_dtor_this[1] == (void*)0x1004) &&
              (g_dtor_this[2] == (void*)0x1000);

    if (ok)
        printf("PASS_00BFEED8_OK\n");
    else
        printf("FAIL_00BFEED8 p=%d e=%d u=%d n=%d t0=%p t1=%p t2=%p\n",
               g_prolog_hit, g_epilog_hit, g_unwind_hit, g_dtor_calls,
               g_dtor_this[0], g_dtor_this[1], g_dtor_this[2]);
    return 0;
}