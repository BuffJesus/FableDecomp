#include "rebuild_abi.h"

// CPersistTraits<std::list<COpinionTransientOffset> >::TransferBinaryOut @ 0x0047931e
//
// Retail is __fastcall / void / 3 params, but the `this` pointer (ecx) is never
// touched by the body.  Modelled as __fastcall so the register/stack layout is
// reproduced exactly:
//   ecx = this (unused)   edx = unused
//   [esp+0x10] = stream/persist-context (ebx)
//   [esp+0x14] = std::list<COpinionTransientOffset> container (edi)
//
// Body: ecx=list; call size-helper -> eax; ecx=stream; stream->PutCount(eax);
// then walk the circular list from head->next calling stream->vtbl[4](&node.value,
// 0x18) for each value node until we cycle back to the sentinel.  ret 8.
//
// Authored as naked asm to guarantee the exact retail byte stream.  The two
// relative calls resolve to external stubs; their rel32 operands are
// parity-masked so the concrete targets do not matter.

extern "C" fable_u32 FABLE_FASTCALL
CPersistTraits_OpinionList_Size_0047931e(void* list_ecx);

extern "C" void FABLE_FASTCALL
CPersistTraits_OpinionStream_PutCount_0047931e(void* stream_ecx, fable_u32 count);

extern "C" __declspec(naked) void FABLE_FASTCALL
CPersistTraits_OpinionList_TransferBinaryOut_0047931e(
    void* self_ecx, void* edx, void* stream, void* list)
{
    (void)self_ecx; (void)edx; (void)stream; (void)list;
    __asm
    {
        push ebx
        push esi
        push edi
        mov  edi, dword ptr [esp+0x14]
        mov  ecx, edi
        call CPersistTraits_OpinionList_Size_0047931e
        mov  ebx, dword ptr [esp+0x10]
        push eax
        mov  ecx, ebx
        call CPersistTraits_OpinionStream_PutCount_0047931e
        mov  eax, dword ptr [edi]
        mov  esi, dword ptr [eax]
        cmp  esi, eax
        je   done

    walk:
        mov  eax, dword ptr [ebx]
        push 0x18
        lea  ecx, [esi+0x8]
        push ecx
        mov  ecx, ebx
        call dword ptr [eax+0x10]
        mov  esi, dword ptr [esi]
        cmp  esi, dword ptr [edi]
        jne  walk

    done:
        pop  edi
        pop  esi
        pop  ebx
        ret  8
    }
}