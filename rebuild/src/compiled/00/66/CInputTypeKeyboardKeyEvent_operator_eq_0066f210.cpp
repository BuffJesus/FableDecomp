extern "C" __declspec(naked) unsigned char __fastcall
CInputTypeKeyboardKeyEvent_operator_eq_0066f210(
    void* event, void*, const void* other)
{
    __asm
    {
        push ebx
        push esi
        push edi
        mov edi, [esp + 10h]
        mov eax, [edi]
        mov esi, ecx
        mov ecx, edi
        call dword ptr [eax + 4]
        mov edx, [esi]
        mov ecx, esi
        mov ebx, eax
        call dword ptr [edx + 4]
        cmp eax, ebx
        jne not_equal
        mov eax, [esi + 4]
        sub eax, [edi + 4]
        neg eax
        pop edi
        sbb eax, eax
        pop esi
        inc eax
        pop ebx
        ret 4

    not_equal:
        pop edi
        pop esi
        xor al, al
        pop ebx
        ret 4
    }
}
