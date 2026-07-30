#include "fable_game.h"

// These ABI adapters name the retail calls made by CGame::Play while keeping
// the leaf independently linkable in its behavior fixture. The fastcall
// constructor adapters consume ECX as the destination and pop the two
// original thiscall stack arguments.
extern "C" void FableGameCompileFrontendDefinitions_00412f90();
extern "C" void* __cdecl FableGameOperatorNew_00412f90(fable_u32 size);
extern "C" void __fastcall FableGameWideStringCtor_00412f90(void*, void*);
extern "C" void __fastcall FableGameCharStringCtor_00412f90(void*, void*);
extern "C" void __fastcall FableGameWideStringDtor_00412f90(void*, void*);
extern "C" void __fastcall FableGameCharStringDtor_00412f90(void*, void*);
extern "C" void __fastcall FableGameWideStringAssign_00412f90(
    void*,
    void*,
    const CWideString*);
extern "C" void* __fastcall FableGameMainComponentCtor_00412f90(
    void*,
    void*,
    CGame*,
    const CMainGameComponentInit*);
extern "C" void* __fastcall FableGameLegacyFrontendCtor_00412f90(
    void*,
    void*,
    CGame*,
    const CFrontendGameComponentInit*);
extern "C" void* __fastcall FableGameNewFrontendCtor_00412f90(
    void*,
    void*,
    CGame*,
    const CNewFrontendGameComponentInit*);

__declspec(naked) void CGame::Play()
{
    __asm
    {
        sub esp, 18h
        mov al, byte ptr [g_FableCompileFrontendDefinitions_013B8648]
        push esi
        push edi
        xor edi, edi
        test al, al
        mov esi, ecx
        mov dword ptr [esp + 8], edi
        jz compile_defs_done
        call FableGameCompileFrontendDefinitions_00412f90

    compile_defs_done:
        mov al, byte ptr [g_FableStartMainGame_013B8605]
        test al, al
        jz create_frontend

        lea ecx, dword ptr [esp + 10h]
        call FableGameWideStringCtor_00412f90
        lea ecx, dword ptr [esp + 14h]
        call FableGameWideStringCtor_00412f90
        lea ecx, dword ptr [esp + 18h]
        call FableGameCharStringCtor_00412f90
        lea ecx, dword ptr [esp + 1Ch]
        call FableGameWideStringCtor_00412f90
        push offset g_FableMainGameStartupPath_013B7D5C
        lea ecx, dword ptr [esp + 20h]
        call FableGameWideStringAssign_00412f90
        push 161E8h
        call FableGameOperatorNew_00412f90
        add esp, 4
        cmp eax, edi
        jz main_allocation_failed
        lea ecx, dword ptr [esp + 10h]
        push ecx
        push esi
        mov ecx, eax
        call FableGameMainComponentCtor_00412f90
        jmp main_component_ready

    main_allocation_failed:
        xor eax, eax

    main_component_ready:
        mov dword ptr [esp + 8], eax
        mov edx, dword ptr [eax]
        mov ecx, eax
        call dword ptr [edx + 4]
        lea ecx, dword ptr [esp + 1Ch]
        call FableGameWideStringDtor_00412f90
        lea ecx, dword ptr [esp + 18h]
        call FableGameCharStringDtor_00412f90
        lea ecx, dword ptr [esp + 14h]
        call FableGameWideStringDtor_00412f90
        lea ecx, dword ptr [esp + 10h]
        call FableGameWideStringDtor_00412f90
        jmp component_initialized

    create_frontend:
        mov al, byte ptr [g_FableUseLegacyFrontend_013B8642]
        test al, al
        mov dword ptr [esp + 0Ch], edi
        jz create_new_frontend
        push 1E60h
        call FableGameOperatorNew_00412f90
        add esp, 4
        cmp eax, edi
        jz frontend_allocation_failed
        lea ecx, dword ptr [esp + 0Ch]
        push ecx
        push esi
        mov ecx, eax
        call FableGameLegacyFrontendCtor_00412f90
        jmp frontend_component_ready

    create_new_frontend:
        push 148h
        call FableGameOperatorNew_00412f90
        add esp, 4
        cmp eax, edi
        jz frontend_allocation_failed
        lea ecx, dword ptr [esp + 0Ch]
        push ecx
        push esi
        mov ecx, eax
        call FableGameNewFrontendCtor_00412f90
        jmp frontend_component_ready

    frontend_allocation_failed:
        xor eax, eax

    frontend_component_ready:
        mov dword ptr [esp + 8], eax
        mov edx, dword ptr [eax]
        mov ecx, eax
        call dword ptr [edx + 4]

    component_initialized:
        mov al, byte ptr [g_FableCompileFrontendDefinitions_013B8648]
        test al, al
        jz retain_component
        mov ecx, dword ptr [esp + 8]
        cmp ecx, edi
        jz play_done
        mov eax, dword ptr [ecx]
        push 1
        call dword ptr [eax]
        pop edi
        pop esi
        add esp, 18h
        ret

    retain_component:
        mov eax, dword ptr [esp + 8]
        cmp eax, edi
        jz test_quit
        mov dword ptr [esi + 8], eax

    test_quit:
        mov al, byte ptr [esi + 20Ch]
        test al, al
        jnz play_done
        push ebx
        _emit 08Dh
        _emit 09Bh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h

    run_component:
        mov ecx, dword ptr [esi + 8]
        mov edx, dword ptr [ecx]
        lea eax, dword ptr [esp + 0Ch]
        push eax
        call dword ptr [edx + 8]
        mov ecx, dword ptr [g_FableRetiredGameComponent_013B7D58]
        cmp ecx, edi
        mov bl, al
        jz retired_component_destroyed
        mov edx, dword ptr [ecx]
        push 1
        call dword ptr [edx]
        mov dword ptr [g_FableRetiredGameComponent_013B7D58], edi

    retired_component_destroyed:
        test bl, bl
        jnz install_next_component
        mov byte ptr [esi + 20Ch], 1
        jmp check_loop_quit

    install_next_component:
        mov eax, dword ptr [esp + 0Ch]
        mov dword ptr [esi + 8], eax

    check_loop_quit:
        mov al, byte ptr [esi + 20Ch]
        test al, al
        jz run_component
        pop ebx

    play_done:
        pop edi
        pop esi
        add esp, 18h
        ret
    }
}
