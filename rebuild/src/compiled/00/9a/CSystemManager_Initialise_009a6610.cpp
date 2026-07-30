#include "fable_gfuninitialise.h"

extern "C" void* (FABLE_STDCALL* g_FableConvertThreadToFiber_009a6610)(void*);
extern "C" long (FABLE_STDCALL* g_FableCoInitialize_009a6610)(void*);
extern "C" void (FABLE_STDCALL* g_FableInitializeCriticalSection_009a6610)(void*);
extern "C" void* (FABLE_STDCALL* g_FableGetCurrentProcess_009a6610)();

extern "C" fable_u8 g_FableDriveManagerVTable_0129AFF4;
extern "C" void* g_FableDriveManagerPointer_013CAA28;
extern "C" fable_u8 g_FableScratchBuffer_013CB498;
extern "C" fable_u8 g_FableSystemInitialiseCount_013CA608;

extern "C" bool __fastcall FableSystemPreInitialise_009a6610(void*, void*);
extern "C" void __fastcall FableSystemWideAssign_009a6610(
    void*,
    void*,
    const void*);
extern "C" void __fastcall FableSystemInitGuid_009a6610(
    void*,
    void*,
    const void*);
extern "C" void FableSystemDefaultErrorCallback_009a6610();
extern "C" bool __fastcall FableSystemInitWindow_009a6610(
    void*,
    void*,
    fable_u32,
    fable_u32);
extern "C" void __fastcall FableSystemUninitialise_009a6610(void*, void*);
extern "C" void __fastcall FableSystemScratchInitialise_009a6610(
    void*,
    void*,
    fable_u32);
extern "C" void __fastcall FableSystemDisplayInitCopy_009a6610(
    void*,
    void*,
    const void*);
extern "C" void* __cdecl FableSystemOperatorNew_009a6610(fable_u32);
extern "C" void* __fastcall FableSystemDisplayConstruct_009a6610(
    void*,
    void*,
    void*);
extern "C" void __fastcall FableSystemDisplayReset_009a6610(
    void*,
    void*,
    void*);
extern "C" void __fastcall FableSystemDisplayInitialise_009a6610(
    void*,
    void*,
    void*,
    fable_u32);
extern "C" void __fastcall FableSystemCharStringDestroy_009a6610(void*, void*);
extern "C" void* __fastcall FableSystemInputConstruct_009a6610(
    void*,
    void*,
    const void*,
    fable_u32);
extern "C" void __fastcall FableSystemInputReset_009a6610(
    void*,
    void*,
    void*);
extern "C" void __fastcall FableSystemDriveReset_009a6610(
    void*,
    void*,
    void*);
extern "C" ESystemCode __fastcall FableSystemDriveInitialise_009a6610(
    void*,
    void*,
    void*);
extern "C" void* __fastcall FableSystemProfileConstruct_009a6610(
    void*,
    void*,
    const void*,
    fable_u32);
extern "C" void __fastcall FableSystemProfileReset_009a6610(
    void*,
    void*,
    void*);
extern "C" void* __fastcall FableSystemFontConstruct_009a6610(
    void*,
    void*,
    const void*,
    const void*);
extern "C" void __fastcall FableSystemFontReset_009a6610(
    void*,
    void*,
    void*);
extern "C" void* __fastcall FableSystemTextConstruct_009a6610(void*, void*);
extern "C" void __fastcall FableSystemTextReset_009a6610(
    void*,
    void*,
    void*);
extern "C" void* __fastcall FableSystemMeshConstruct_009a6610(void*, void*);
extern "C" void __fastcall FableSystemMeshReset_009a6610(
    void*,
    void*,
    void*);
extern "C" void __fastcall FableSystemAddComponent_009a6610(
    void*,
    void*,
    void*);
extern "C" void* __fastcall FableSystemColoursConstruct_009a6610(void*, void*);
extern "C" void __fastcall FableSystemColoursReset_009a6610(
    void*,
    void*,
    void*);
extern "C" void FableSystemInitialiseCosine_009a6610();
extern "C" void FableSystemInitialiseVectorMath_009a6610();
extern "C" void __fastcall FableSystemInitColours_009a6610(void*, void*);
extern "C" void __fastcall FableSystemInitApplicationName_009a6610(void*, void*);

__declspec(naked) ESystemCode
CSystemManager::Initialise(CSystemManagerInit*)
{
    __asm
    {
        sub esp, 3Ch
        push ebp
        push esi
        mov esi, ecx
        push edi
        mov edi, dword ptr [esp + 4Ch]
        mov byte ptr [esi + 8Ch], 0
        mov byte ptr [esi + 8Dh], 0
        mov byte ptr [esi + 0E8h], 0
        mov byte ptr [esi + 0E9h], 0
        mov byte ptr [esi + 0EAh], 0
        mov byte ptr [esi + 0DCh], 0
        mov al, byte ptr [edi + 34h]
        mov byte ptr [esi + 0EBh], al
        xor ebp, ebp
        mov dword ptr [esi + 0E4h], ebp
        mov byte ptr [esi + 16Ch], 0
        mov dword ptr [esi + 170h], ebp
        mov cl, byte ptr [edi + 19h]
        mov byte ptr [esi + 0ECh], cl
        mov dl, byte ptr [edi + 20h]
        lea ecx, dword ptr [edi + 0BCh]
        mov byte ptr [esi + 0EDh], dl
        mov al, byte ptr [edi + 10h]
        push ecx
        lea ecx, dword ptr [esi + 164h]
        mov byte ptr [esi + 8Eh], al
        call FableSystemWideAssign_009a6610
        mov edx, dword ptr [edi + 0C0h]
        mov dword ptr [esi + 178h], edx
        mov eax, dword ptr [edi + 0C4h]
        mov ecx, esi
        mov dword ptr [esi + 17Ch], eax
        call FableSystemPreInitialise_009a6610
        cmp al, 1
        jne final_subsystems
        push esi
        call dword ptr [g_FableConvertThreadToFiber_009a6610]
        push ebp
        mov dword ptr [esi + 4Ch], eax
        call dword ptr [g_FableCoInitialize_009a6610]
        lea ecx, dword ptr [esi + 34h]
        push ecx
        call dword ptr [g_FableInitializeCriticalSection_009a6610]
        mov dl, byte ptr [edi + 11h]
        mov byte ptr [esi + 30h], dl
        mov eax, dword ptr [edi]
        mov dword ptr [esi + 90h], eax
        mov ecx, dword ptr [edi + 4]
        mov dword ptr [esi + 9Ch], ecx
        mov edx, dword ptr [edi + 8]
        lea eax, dword ptr [edi + 0Ch]
        push eax
        lea ecx, dword ptr [esi + 0A0h]
        mov dword ptr [esi + 98h], edx
        call FableSystemWideAssign_009a6610
        lea ecx, dword ptr [edi + 0A0h]
        push ecx
        mov ecx, esi
        call FableSystemInitGuid_009a6610
        mov eax, dword ptr [edi + 9Ch]
        cmp eax, ebp
        jne use_error_callback
        mov dword ptr [esi + 174h], offset FableSystemDefaultErrorCallback_009a6610
        jmp error_callback_ready

    use_error_callback:
        mov dword ptr [esi + 174h], eax

    error_callback_ready:
        mov byte ptr [esi + 9], 0
        mov edx, dword ptr [edi + 14h]
        mov dword ptr [esi + 0D8h], edx
        mov eax, dword ptr [edi + 24h]
        mov dword ptr [esi + 0F0h], eax
        mov ecx, dword ptr [edi + 28h]
        mov dword ptr [esi + 0F4h], ecx
        mov dl, byte ptr [edi + 21h]
        mov byte ptr [esi + 0EEh], dl
        test byte ptr [edi + 14h], 4
        jz window_ready
        mov al, byte ptr [esi + 0ECh]
        test al, al
        jne attach_external_window
        mov eax, dword ptr [edi + 60h]
        mov ecx, dword ptr [edi + 5Ch]
        push eax
        push ecx
        mov ecx, esi
        call FableSystemInitWindow_009a6610
        test al, al
        jne mark_window_ready
        mov ecx, esi
        call FableSystemUninitialise_009a6610
        pop edi
        pop esi
        mov eax, 4
        pop ebp
        add esp, 3Ch
        ret 4

    attach_external_window:
        mov edx, dword ptr [edi + 1Ch]
        mov dword ptr [esi + 94h], edx

    mark_window_ready:
        mov byte ptr [esi + 8Ch], 1

    window_ready:
        call dword ptr [g_FableGetCurrentProcess_009a6610]
        mov eax, dword ptr [edi + 0B4h]
        cmp eax, ebp
        jle scratch_ready
        push eax
        mov ecx, offset g_FableScratchBuffer_013CB498
        call FableSystemScratchInitialise_009a6610

    scratch_ready:
        test byte ptr [edi + 14h], 4
        jz display_ready
        lea eax, dword ptr [edi + 5Ch]
        push eax
        lea ecx, dword ptr [esp + 10h]
        call FableSystemDisplayInitCopy_009a6610
        mov ecx, dword ptr [esi + 94h]
        mov dl, byte ptr [edi + 35h]
        push 2C8h
        mov dword ptr [esp + 24h], ecx
        mov byte ptr [esp + 40h], dl
        call FableSystemOperatorNew_009a6610
        add esp, 4
        cmp eax, ebp
        jz display_allocation_failed
        push esi
        mov ecx, eax
        call FableSystemDisplayConstruct_009a6610
        jmp display_object_ready

    display_allocation_failed:
        xor eax, eax

    display_object_ready:
        lea ebp, dword ptr [esi + 60h]
        push eax
        mov ecx, ebp
        call FableSystemDisplayReset_009a6610
        xor eax, eax
        mov al, byte ptr [esi + 8Eh]
        lea ecx, dword ptr [esp + 0Ch]
        push eax
        push ecx
        mov ecx, dword ptr [ebp]
        call FableSystemDisplayInitialise_009a6610
        lea ecx, dword ptr [esp + 44h]
        call FableSystemCharStringDestroy_009a6610

    display_ready:
        test byte ptr [edi + 14h], 1
        jz input_ready
        push 84h
        call FableSystemOperatorNew_009a6610
        add esp, 4
        test eax, eax
        jz input_allocation_failed
        xor edx, edx
        mov dl, byte ptr [esi + 8Eh]
        lea ecx, dword ptr [edi + 58h]
        push edx
        push ecx
        mov ecx, eax
        call FableSystemInputConstruct_009a6610
        jmp input_object_ready

    input_allocation_failed:
        xor eax, eax

    input_object_ready:
        push eax
        lea ecx, dword ptr [esi + 58h]
        call FableSystemInputReset_009a6610

    input_ready:
        push 8
        call FableSystemOperatorNew_009a6610
        add esp, 4
        test eax, eax
        jz drive_allocation_failed
        mov dword ptr [eax], offset g_FableDriveManagerVTable_0129AFF4
        jmp drive_object_ready

    drive_allocation_failed:
        xor eax, eax

    drive_object_ready:
        push eax
        mov ecx, offset g_FableDriveManagerPointer_013CAA28
        call FableSystemDriveReset_009a6610
        mov ecx, dword ptr [g_FableDriveManagerPointer_013CAA28]
        push esi
        call FableSystemDriveInitialise_009a6610
        mov ebp, eax
        cmp ebp, 1
        je drive_ready
        mov ecx, esi
        call FableSystemUninitialise_009a6610
        pop edi
        pop esi
        mov eax, ebp
        pop ebp
        add esp, 3Ch
        ret 4

    drive_ready:
        test byte ptr [edi + 14h], 10h
        jz profile_ready
        push 80h
        call FableSystemOperatorNew_009a6610
        add esp, 4
        test eax, eax
        jz profile_allocation_failed
        xor edx, edx
        mov dl, byte ptr [esi + 8Eh]
        lea ecx, dword ptr [edi + 38h]
        push edx
        push ecx
        mov ecx, eax
        call FableSystemProfileConstruct_009a6610
        jmp profile_object_ready

    profile_allocation_failed:
        xor eax, eax

    profile_object_ready:
        push eax
        lea ecx, dword ptr [esi + 7Ch]
        call FableSystemProfileReset_009a6610

    profile_ready:
        mov eax, dword ptr [edi + 2Ch]
        test eax, eax
        lea ebp, dword ptr [edi + 2Ch]
        jz check_streaming_font
        mov ecx, dword ptr [eax + 4]
        test ecx, ecx
        jnz create_font_manager

    check_streaming_font:
        mov eax, dword ptr [edi + 30h]
        test eax, eax
        jz font_ready
        mov ecx, dword ptr [eax + 4]
        test ecx, ecx
        jz font_ready

    create_font_manager:
        push 3Ch
        call FableSystemOperatorNew_009a6610
        add esp, 4
        test eax, eax
        jz font_allocation_failed
        add edi, 30h
        push edi
        push ebp
        mov ecx, eax
        call FableSystemFontConstruct_009a6610
        jmp font_object_ready

    font_allocation_failed:
        xor eax, eax

    font_object_ready:
        push eax
        lea ecx, dword ptr [esi + 84h]
        call FableSystemFontReset_009a6610

    font_ready:
        push 8
        call FableSystemOperatorNew_009a6610
        add esp, 4
        test eax, eax
        jz text_allocation_failed
        mov ecx, eax
        call FableSystemTextConstruct_009a6610
        jmp text_object_ready

    text_allocation_failed:
        xor eax, eax

    text_object_ready:
        push eax
        lea ecx, dword ptr [esi + 6Ch]
        call FableSystemTextReset_009a6610
        push 0Ch
        call FableSystemOperatorNew_009a6610
        add esp, 4
        test eax, eax
        jz mesh_allocation_failed
        mov ecx, eax
        call FableSystemMeshConstruct_009a6610
        jmp mesh_object_ready

    mesh_allocation_failed:
        xor eax, eax

    mesh_object_ready:
        lea edi, dword ptr [esi + 74h]
        push eax
        mov ecx, edi
        call FableSystemMeshReset_009a6610
        mov ecx, dword ptr [esi + 60h]
        test ecx, ecx
        jz final_subsystems
        mov edi, dword ptr [edi]
        test edi, edi
        jz no_mesh_component
        lea eax, dword ptr [edi + 4]
        jmp add_mesh_component

    no_mesh_component:
        xor eax, eax

    add_mesh_component:
        push eax
        call FableSystemAddComponent_009a6610

    final_subsystems:
        push 78h
        call FableSystemOperatorNew_009a6610
        add esp, 4
        test eax, eax
        jz colours_allocation_failed
        mov ecx, eax
        call FableSystemColoursConstruct_009a6610
        jmp colours_object_ready

    colours_allocation_failed:
        xor eax, eax

    colours_object_ready:
        push eax
        lea ecx, dword ptr [esi + 0F8h]
        call FableSystemColoursReset_009a6610
        call FableSystemInitialiseCosine_009a6610
        call FableSystemInitialiseVectorMath_009a6610
        mov ecx, esi
        call FableSystemInitColours_009a6610
        inc byte ptr [g_FableSystemInitialiseCount_013CA608]
        mov ecx, esi
        call FableSystemInitApplicationName_009a6610
        mov al, byte ptr [esi + 9]
        test al, al
        jne final_flags
        cmp byte ptr [esi + 0DCh], 1
        jne final_flags
        mov byte ptr [esi + 0EAh], 1

    final_flags:
        pop edi
        mov byte ptr [esi + 9], 1
        mov byte ptr [esi + 0DCh], 1
        pop esi
        mov eax, 1
        pop ebp
        add esp, 3Ch
        ret 4
    }
}
