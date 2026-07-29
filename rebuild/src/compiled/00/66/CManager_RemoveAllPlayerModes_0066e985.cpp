extern "C" void* FableRemoveAllPlayerModesGlobal;
extern "C" void* __fastcall
CManager_RemoveAllPlayerModes_0066e985_GetPlayer(void* context, void*);
extern "C" void __fastcall
CManager_RemoveAllPlayerModes_0066e985_GetScreens(
    void* manager, void*, void* output);
extern "C" void __fastcall
CManager_RemoveAllPlayerModes_0066e985_RemoveMode(
    void* player, void*, long mode);
extern "C" void __cdecl
CManager_RemoveAllPlayerModes_0066e985_Free(void* memory);

extern "C" __declspec(naked) void __fastcall
CManager_RemoveAllPlayerModes_0066e985(void* manager, void*)
{
    __asm
    {
        push ebp
        mov ebp, esp
        sub esp, 10h
        mov eax, dword ptr [FableRemoveAllPlayerModesGlobal]
        push ebx
        push esi
        mov esi, ecx
        mov ecx, [eax + 1Ch]
        push edi
        call CManager_RemoveAllPlayerModes_0066e985_GetPlayer
        mov [ebp - 4], eax
        xor edi, edi
        lea eax, [ebp - 10h]
        push eax
        mov ecx, esi
        mov [ebp - 10h], edi
        mov [ebp - 0Ch], edi
        mov [ebp - 8], edi
        call CManager_RemoveAllPlayerModes_0066e985_GetScreens
        mov esi, [ebp - 0Ch]
        mov ebx, [ebp - 10h]
        sub esi, ebx
        sar esi, 2
        je free_vector

    remove_modes:
        mov ecx, [ebx + edi * 4]
        mov eax, [ecx]
        call dword ptr [eax + 4]
        mov ecx, [ebp - 4]
        push eax
        call CManager_RemoveAllPlayerModes_0066e985_RemoveMode
        inc edi
        cmp edi, esi
        jb remove_modes

    free_vector:
        test ebx, ebx
        je done
        push ebx
        call CManager_RemoveAllPlayerModes_0066e985_Free
        pop ecx

    done:
        pop edi
        pop esi
        pop ebx
        leave
        ret
    }
}
