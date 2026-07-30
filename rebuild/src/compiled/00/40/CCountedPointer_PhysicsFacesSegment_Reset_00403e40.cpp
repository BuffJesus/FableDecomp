#include "rebuild_abi.h"

struct FablePhysicsFacesControl_00403E40
{
    fable_i32 owners;
    void (FABLE_FASTCALL *destroyObject)(void*, void*);
    const void* object;
};

struct FablePhysicsFacesCountedPointer_00403E40
{
    const void* object;
    FablePhysicsFacesControl_00403E40* control;
};

extern "C" void* FABLE_CDECL
FablePhysicsFacesControlAllocate_00403E40(fable_u32 size);
extern "C" void FABLE_CDECL
FablePhysicsFacesControlDelete_00403E40(void* control);
extern "C" void FABLE_FASTCALL
FablePhysicsFacesObjectDelete_00403650(void* object, void*);

// CCountedPointer<CIPhysicsFacesSegmentBase const>::Reset(
//     CIPhysicsFacesSegmentBase const*) @ 0x00403E40.
extern "C" __declspec(naked) void FABLE_FASTCALL
FablePhysicsFacesCountedPointerReset_00403E40(
    FablePhysicsFacesCountedPointer_00403E40*,
    void*,
    const void*)
{
    __asm
    {
        push esi
        mov esi, ecx
        mov eax, dword ptr [esi + 4]
        test eax, eax
        je old_owner_released
        dec dword ptr [eax]
        mov eax, dword ptr [esi + 4]
        cmp dword ptr [eax], 0
        jne old_owner_released
        mov ecx, dword ptr [eax + 8]
        call dword ptr [eax + 4]
        mov eax, dword ptr [esi + 4]
        push eax
        call FablePhysicsFacesControlDelete_00403E40
        add esp, 4

    old_owner_released:
        mov eax, dword ptr [esp + 8]
        test eax, eax
        mov dword ptr [esi + 4], 0
        mov dword ptr [esi], eax
        je finished
        push 0Ch
        call FablePhysicsFacesControlAllocate_00403E40
        add esp, 4
        test eax, eax
        je allocation_failed
        mov ecx, dword ptr [esi]
        mov dword ptr [eax], 1
        mov dword ptr [eax + 4], offset FablePhysicsFacesObjectDelete_00403650
        mov dword ptr [eax + 8], ecx
        mov dword ptr [esi + 4], eax
        pop esi
        ret 4

    allocation_failed:
        xor eax, eax
        mov dword ptr [esi + 4], eax

    finished:
        pop esi
        ret 4
    }
}
