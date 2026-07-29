extern "C" unsigned char FableDeregisterScreenGuard;
extern "C" void __fastcall
CManager_DeregisterScreen_0066eb78_InitialiseScrollIndices(
    void* manager, void*, long count);

extern "C" __declspec(naked) void __fastcall
CManager_DeregisterScreen_0066eb78(void* manager, void*, void* screen)
{
    __asm
    {
        cmp byte ptr [FableDeregisterScreenGuard], 0
        push esi
        mov esi, ecx
        jne done
        push edi
        mov edi, [esp + 0Ch]
        mov eax, [edi]
        mov ecx, edi
        call dword ptr [eax + 8]
        mov ecx, [esi + 4]
        lea eax, [ecx + eax * 4]
        cmp [eax], edi
        jne count_screens
        and dword ptr [eax], 0

    count_screens:
        mov eax, [esi + 8]
        sub eax, [esi + 4]
        xor edx, edx
        xor ecx, ecx
        sar eax, 2
        je compare_scroll_count
        mov eax, [esi + 4]

    count_registered:
        cmp dword ptr [eax], 0
        je next
        inc edx

    next:
        mov edi, [esi + 8]
        sub edi, [esi + 4]
        inc ecx
        sar edi, 2
        add eax, 4
        cmp ecx, edi
        jb count_registered

    compare_scroll_count:
        mov eax, [esi + 20h]
        sub eax, [esi + 1Ch]
        pop edi
        sar eax, 2
        cmp edx, eax
        jae done
        push edx
        mov ecx, esi
        call CManager_DeregisterScreen_0066eb78_InitialiseScrollIndices

    done:
        pop esi
        ret 4
    }
}
