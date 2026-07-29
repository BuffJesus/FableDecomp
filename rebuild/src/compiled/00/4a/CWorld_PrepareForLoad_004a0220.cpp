// CWorld::PrepareForLoad @ 0x004A0220.
//
// This is the destructive pre-load boundary reached only after save signature
// validation.  It clears player/UI/world subsystems, releases the current
// counted world object, resets persistent load flags, and empties the pending
// range before LoadGameStateInternal begins deserializing the new state.

extern "C" void* CWorld_PrepareForLoad_Global_004a0220;

extern "C" void* __fastcall CWorld_Prepare_GetPlayer_004a0220(void*, void*);
extern "C" void __fastcall
CWorld_Prepare_RemovePlayerMode_004a0220(void*, void*, int);
extern "C" void __fastcall CWorld_Prepare_ResetPlayer_004a0220(void*, void*);
extern "C" void* __fastcall
CWorld_Prepare_GetEnvironment_004a0220(void*, void*);
extern "C" void __fastcall
CWorld_Prepare_ResetEnvironment_004a0220(void*, void*);
extern "C" void __fastcall
CWorld_Prepare_ResetGlobalInterface_004a0220(void*, void*);
extern "C" void __fastcall CWorld_Prepare_Subsystem1_004a0220(void*, void*);
extern "C" void __fastcall CWorld_Prepare_Subsystem2_004a0220(void*, void*);
extern "C" void __fastcall CWorld_Prepare_Subsystem3_004a0220(void*, void*);
extern "C" void __fastcall
CWorld_Prepare_Subsystem4_004a0220(void*, void*, int);
extern "C" void __fastcall CWorld_Prepare_Subsystem5_004a0220(void*, void*);
extern "C" void __fastcall
CWorld_Prepare_SetEnvironmentState_004a0220(void*, void*, int);
extern "C" void __cdecl CWorld_Prepare_DeleteCountedBlock_004a0220(void*);
extern "C" void __fastcall
CWorld_Prepare_SetTheme_004a0220(void*, void*, int, int);
extern "C" void __fastcall CWorld_Prepare_WorldMap_004a0220(void*, void*);
extern "C" void __fastcall CWorld_Prepare_Subsystem6_004a0220(void*, void*);
extern "C" void __fastcall CWorld_Prepare_PlayerGui_004a0220(void*, void*);
extern "C" void __fastcall CWorld_Prepare_Subsystem7_004a0220(void*, void*);
extern "C" void __fastcall
CWorld_Prepare_EraseRange_004a0220(
    void* range,
    void*,
    void* first,
    void* last);

extern "C" __declspec(naked) void __fastcall
CWorld_PrepareForLoad_004a0220(void*, void*)
{
    __asm
    {
        push ebx
        push esi
        mov esi, ecx

        mov ecx, dword ptr [esi + 0Ch]
        push 2Bh
        call CWorld_Prepare_GetPlayer_004a0220
        mov ecx, eax
        call CWorld_Prepare_RemovePlayerMode_004a0220

        mov ecx, dword ptr [esi + 0Ch]
        call CWorld_Prepare_GetPlayer_004a0220
        mov ecx, eax
        call CWorld_Prepare_ResetPlayer_004a0220

        mov ecx, dword ptr [CWorld_PrepareForLoad_Global_004a0220]
        call CWorld_Prepare_GetEnvironment_004a0220
        mov ecx, eax
        call CWorld_Prepare_ResetEnvironment_004a0220

        mov ecx, dword ptr [CWorld_PrepareForLoad_Global_004a0220]
        call CWorld_Prepare_ResetGlobalInterface_004a0220

        mov ecx, dword ptr [esi + 6Ch]
        call CWorld_Prepare_Subsystem1_004a0220
        mov ecx, dword ptr [esi + 6Ch]
        call CWorld_Prepare_Subsystem2_004a0220
        mov ecx, dword ptr [esi + 6Ch]
        call CWorld_Prepare_Subsystem3_004a0220
        mov ecx, dword ptr [esi + 6Ch]
        xor ebx, ebx
        push ebx
        call CWorld_Prepare_Subsystem4_004a0220
        mov ecx, dword ptr [esi + 6Ch]
        call CWorld_Prepare_Subsystem5_004a0220

        mov ecx, dword ptr [esi + 18h]
        push 1
        call CWorld_Prepare_SetEnvironmentState_004a0220

        mov eax, dword ptr [esi + 28h]
        cmp eax, ebx
        je counted_release_done
        dec dword ptr [eax]
        mov eax, dword ptr [esi + 28h]
        cmp dword ptr [eax], ebx
        jne counted_release_done
        mov ecx, dword ptr [eax + 8]
        call dword ptr [eax + 4]
        mov eax, dword ptr [esi + 28h]
        push eax
        call CWorld_Prepare_DeleteCountedBlock_004a0220
        add esp, 4

    counted_release_done:
        push ebx
        mov dword ptr [esi + 28h], ebx
        mov dword ptr [esi + 24h], ebx
        mov ecx, dword ptr [esi + 18h]
        push 1
        call CWorld_Prepare_SetTheme_004a0220
        mov ecx, dword ptr [esi + 18h]
        push ebx
        push 2
        call CWorld_Prepare_SetTheme_004a0220
        mov ecx, dword ptr [esi + 18h]
        push ebx
        push 3
        call CWorld_Prepare_SetTheme_004a0220

        mov edx, dword ptr [esi]
        mov ecx, esi
        call dword ptr [edx + 30h]
        mov ecx, eax
        call CWorld_Prepare_WorldMap_004a0220

        mov ecx, dword ptr [esi + 68h]
        call CWorld_Prepare_Subsystem6_004a0220

        mov eax, dword ptr [esi + 8]
        mov ecx, dword ptr [eax + 28h]
        call CWorld_Prepare_PlayerGui_004a0220

        mov ecx, dword ptr [esi + 60h]
        add ecx, 4
        call CWorld_Prepare_Subsystem7_004a0220

        lea ecx, dword ptr [esi + 168h]
        mov dword ptr [esi + 8Ch], ebx
        mov byte ptr [esi + 0D8h], bl
        mov byte ptr [esi + 0DBh], 1
        mov byte ptr [esi + 0DCh], bl
        mov byte ptr [esi + 0DFh], 1
        mov byte ptr [esi + 0E2h], bl
        mov dword ptr [esi + 0E4h], ebx
        mov dword ptr [esi + 124h], ebx
        mov byte ptr [esi + 140h], bl
        mov byte ptr [esi + 164h], bl
        mov edx, dword ptr [ecx + 4]
        mov eax, dword ptr [ecx]
        push edx
        push eax
        call CWorld_Prepare_EraseRange_004a0220
        mov dword ptr [esi + 178h], ebx
        pop esi
        pop ebx
        ret
    }
}
