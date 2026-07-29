extern "C" void* __fastcall
CManager_GetScreenAtScrollIndex_0066e7e1(void* manager, void*, long scroll_index);

extern "C" __declspec(naked) void __fastcall
CManager_ProcessPreviousScreenAction_0066e8a1(
    void* manager, void*, long screen_count, long scroll_count)
{
    __asm
    {
        push esi
        mov esi, ecx
        dec dword ptr [esi + 30h]
        jns done
        and dword ptr [esi + 30h], 0
        push edi
        xor edi, edi
        cmp [esp + 0Ch], edi
        jle pop_edi

    previous_screen:
        mov eax, [esi + 1Ch]
        lea eax, [eax + edi * 4]
        dec dword ptr [eax]
        mov eax, [esi + 1Ch]
        lea eax, [eax + edi * 4]
        cmp dword ptr [eax], 0
        jge get_screen
        mov ecx, [esp + 10h]
        add [eax], ecx

    get_screen:
        push edi
        mov ecx, esi
        call CManager_GetScreenAtScrollIndex_0066e7e1
        mov edx, [eax]
        mov ecx, eax
        call dword ptr [edx + 0Ch]
        test al, al
        je previous_screen
        inc edi
        cmp edi, [esp + 0Ch]
        jl previous_screen

    pop_edi:
        pop edi

    done:
        pop esi
        ret 8
    }
}
