extern "C" unsigned char FableRegisterScreenGuard;
extern "C" void __fastcall
CManager_RegisterScreen_0066eb11_InitialiseScrollIndices(
    void* manager, void*, long count);

extern "C" __declspec(naked) void __fastcall
CManager_RegisterScreen_0066eb11(void* manager, void*, void* screen)
{
    __asm
    {
        cmp byte ptr [FableRegisterScreenGuard], 0
        push esi
        mov esi, ecx
        jne done
        push edi
        mov edi, [esp + 0Ch]
        mov eax, [edi]
        mov ecx, edi
        call dword ptr [eax + 8]
        mov ecx, [esi + 4]
        mov [ecx + eax * 4], edi
        mov eax, [esi + 20h]
        sub eax, [esi + 1Ch]
        sar eax, 2
        cmp eax, 5
        jae pop_edi
        mov eax, [esi + 8]
        sub eax, [esi + 4]
        xor edx, edx
        xor ecx, ecx
        sar eax, 2
        je initialise
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
        cmp edx, 5
        jg pop_edi

    initialise:
        push edx
        mov ecx, esi
        call CManager_RegisterScreen_0066eb11_InitialiseScrollIndices

    pop_edi:
        pop edi

    done:
        pop esi
        ret 4
    }
}
