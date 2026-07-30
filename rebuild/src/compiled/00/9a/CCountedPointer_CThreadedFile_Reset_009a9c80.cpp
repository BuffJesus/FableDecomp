#include "fable_threaded_file.h"

struct FableThreadedFileControl_009A9C80
{
    fable_i32 owners;
    void (FABLE_FASTCALL *destroyObject)(void*, void*);
    void* object;
};

extern "C" void* FABLE_CDECL
FableThreadedFileControlAllocate_009A9C80(fable_u32 size);
extern "C" void FABLE_CDECL
FableThreadedFileControlDelete_009A9C80(void* control);
extern "C" void FABLE_FASTCALL
FableThreadedFileObjectDelete_009A9040(void* object, void*);

// CCountedPointer<CThreadedFile>::Reset(CThreadedFile*) @ 0x009A9C80.
__declspec(naked) void
CCountedPointer<CThreadedFile>::Reset(CThreadedFile*)
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
        call FableThreadedFileControlDelete_009A9C80
        add esp, 4

    old_owner_released:
        mov eax, dword ptr [esp + 8]
        test eax, eax
        mov dword ptr [esi + 4], 0
        mov dword ptr [esi], eax
        je finished
        push 0Ch
        call FableThreadedFileControlAllocate_009A9C80
        add esp, 4
        test eax, eax
        je allocation_failed
        mov ecx, dword ptr [esi]
        mov dword ptr [eax], 1
        mov dword ptr [eax + 4], offset FableThreadedFileObjectDelete_009A9040
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
