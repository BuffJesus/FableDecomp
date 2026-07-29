extern "C" void* __fastcall
CManager_GetScreenAtScrollIndex_0066e7e1(void* manager, void*, long scroll_index);

extern "C" __declspec(naked) void __fastcall
CManager_ProcessNextScreenAction_0066e845(
    void* manager, void*, long screen_count, long scroll_count)
{
    __asm
    {
        mov edx, [esp + 4]
        push esi
        mov esi, ecx
        inc dword ptr [esi + 30h]
        mov ecx, [esi + 30h]
        lea eax, [edx - 1]
        cmp ecx, eax
        jle done
        push ebx
        xor ebx, ebx
        test edx, edx
        mov [esi + 30h], eax
        jle pop_ebx
        push edi
        mov edi, [esp + 14h]
        dec edi

    advance_screen:
        mov eax, [esi + 1Ch]
        lea eax, [eax + ebx * 4]
        inc dword ptr [eax]
        mov eax, [esi + 1Ch]
        lea eax, [eax + ebx * 4]
        cmp [eax], edi
        jle get_screen
        mov ecx, [esp + 14h]
        sub [eax], ecx

    get_screen:
        push ebx
        mov ecx, esi
        call CManager_GetScreenAtScrollIndex_0066e7e1
        mov edx, [eax]
        mov ecx, eax
        call dword ptr [edx + 0Ch]
        test al, al
        je advance_screen
        inc ebx
        cmp ebx, [esp + 10h]
        jl advance_screen
        pop edi

    pop_ebx:
        pop ebx

    done:
        pop esi
        ret 8
    }
}
