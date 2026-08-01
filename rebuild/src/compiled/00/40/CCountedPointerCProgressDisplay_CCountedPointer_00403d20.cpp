#include "rebuild_abi.h"

// CCountedPointer<CProgressDisplay>::CCountedPointer(CProgressDisplay*) @ 0x00403D20.
//
// __fastcall constructor: this in ecx, the single object argument on the
// stack. The retail body stores the object in this->object, and when the
// object is non-null allocates a 12-byte reference-control block, seeds it
// with refcount 1, a masked object-destroy function pointer, and the object,
// then installs it as this->control. On a null object or a failed allocation
// this->control is cleared. The constructor returns this in eax and pops the
// single stack argument (ret 4).

struct FableProgressDisplayControl_00403D20
{
    fable_i32 owners;
    void (FABLE_FASTCALL *destroyObject)(void*, void*);
    const void* object;
};

struct FableProgressDisplayCountedPointer_00403D20
{
    const void* object;
    FableProgressDisplayControl_00403D20* control;
};

extern "C" void* FABLE_CDECL
FableProgressDisplayControlAllocate_00403D20(fable_u32 size);
extern "C" void FABLE_FASTCALL
FableProgressDisplayObjectDelete_00403600(void* object, void*);

extern "C" __declspec(naked) FableProgressDisplayCountedPointer_00403D20* FABLE_FASTCALL
FableProgressDisplayCountedPointerCtor_00403D20(
    FableProgressDisplayCountedPointer_00403D20*,
    void*,
    const void*)
{
    __asm
    {
        mov eax, dword ptr [esp + 4]
        test eax, eax
        push esi
        mov esi, ecx
        mov dword ptr [esi], eax
        je null_object
        push 0Ch
        call FableProgressDisplayControlAllocate_00403D20
        add esp, 4
        test eax, eax
        je allocation_failed
        mov ecx, dword ptr [esi]
        mov dword ptr [eax], 1
        mov dword ptr [eax + 4], offset FableProgressDisplayObjectDelete_00403600
        mov dword ptr [eax + 8], ecx
        mov dword ptr [esi + 4], eax
        mov eax, esi
        pop esi
        ret 4

    allocation_failed:
        xor eax, eax
        mov dword ptr [esi + 4], eax
        mov eax, esi
        pop esi
        ret 4

    null_object:
        mov dword ptr [esi + 4], 0
        mov eax, esi
        pop esi
        ret 4
    }
}