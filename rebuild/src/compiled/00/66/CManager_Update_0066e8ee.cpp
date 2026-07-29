extern "C" void __fastcall
CManager_ProcessNextScreenAction_0066e845(
    void* manager, void*, long scroll_count, long screen_count);
extern "C" void __fastcall
CManager_ProcessPreviousScreenAction_0066e8a1(
    void* manager, void*, long scroll_count, long screen_count);

extern "C" __declspec(naked) void __fastcall
CManager_Update_0066e8ee(void* manager, void*)
{
    __asm
    {
        push esi
        mov esi, ecx
        mov ecx, [esi + 20h]
        mov eax, [esi + 14h]
        sub ecx, [esi + 1Ch]
        sub eax, [esi + 10h]
        mov edx, [esi + 2Ch]
        push edi
        mov edi, [esi + 28h]
        sar ecx, 2
        sar eax, 2
        cmp edi, edx
        je done
        cmp edx, eax
        jl compare_direction
        test edi, edi
        jne compare_direction

    process_next:
        push eax
        push ecx
        mov ecx, esi
        call CManager_ProcessNextScreenAction_0066e845
        jmp commit

    compare_direction:
        test edx, edx
        jne compare_indices
        cmp edi, eax
        jge process_previous

    compare_indices:
        cmp edi, edx
        jg process_next
        jge commit

    process_previous:
        push eax
        push ecx
        mov ecx, esi
        call CManager_ProcessPreviousScreenAction_0066e8a1

    commit:
        mov eax, [esi + 28h]
        mov [esi + 2Ch], eax

    done:
        pop edi
        pop esi
        ret
    }
}
