extern "C" __declspec(naked) void* __fastcall
CManager_GetScreenAtScrollIndex_0066e7e1(void* manager, void*, long scroll_index)
{
    __asm
    {
        mov edx, [esp + 4]
        mov eax, [ecx + 1Ch]
        mov eax, [eax + edx * 4]
        mov edx, [ecx + 14h]
        sub edx, [ecx + 10h]
        sar edx, 2
        cmp eax, edx
        jae invalid
        mov ecx, [ecx + 10h]
        mov eax, [ecx + eax * 4]
        jmp done

    invalid:
        xor eax, eax

    done:
        ret 4
    }
}
