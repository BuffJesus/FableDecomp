#include "rebuild_abi.h"

// NUISystem::WaypointData::~WaypointData @ 0x00472B63
// VC7.1, x86, /O2 /Oy.
//
// this-in-ecx dtor taking one stack argument (ret 4).  Body builds a 0x40-byte
// temporary at [ebp-0x40], constructs it, stamps its head dword with a vtable
// pointer, invokes a member on `this` (esi) with the temp + the stack argument,
// then re-stamps the temp head with a second vtable pointer and destructs it.
// All call rel32 targets and the two immediate vtable addresses are
// relocation-masked in parity.
//
// VC7.1 forbids the literal thiscall keyword (C4234), so the callees are
// declared __fastcall: their first argument arrives in ecx exactly as a
// this-in-ecx method expects, and the naked body loads ecx explicitly before
// every call.  The emitted bytes are unaffected by the C-level convention of
// the externs since the body is hand-written asm.

extern "C" void FABLE_FASTCALL
FableWaypointDataTempConstruct_00472B63(void* temp, void* edx);

extern "C" void FABLE_FASTCALL
FableWaypointDataApply_00472B63(void* self, void* edx, void* stackArg, void* temp);

extern "C" void FABLE_FASTCALL
FableWaypointDataTempDestroy_00472B63(void* temp, void* edx);

extern "C" void* const FableWaypointDataApplyVtable_00472B63[];
extern "C" void* const FableWaypointDataDestroyVtable_00472B63[];

extern "C" __declspec(naked) void FABLE_FASTCALL
FableWaypointDataDestroy_00472B63(void* /*ecx this*/, void* /*edx unused*/,
                                  void* /*stack arg @ [ebp+8]*/)
{
    __asm
    {
        push ebp
        mov ebp, esp
        sub esp, 40h
        push esi
        mov esi, ecx
        lea ecx, [ebp-40h]
        call FableWaypointDataTempConstruct_00472B63
        lea eax, [ebp-40h]
        push eax
        push dword ptr [ebp+8]
        mov ecx, esi
        mov dword ptr [ebp-40h], offset FableWaypointDataApplyVtable_00472B63
        call FableWaypointDataApply_00472B63
        lea ecx, [ebp-40h]
        mov dword ptr [ebp-40h], offset FableWaypointDataDestroyVtable_00472B63
        call FableWaypointDataTempDestroy_00472B63
        pop esi
        leave
        ret 4
    }
}