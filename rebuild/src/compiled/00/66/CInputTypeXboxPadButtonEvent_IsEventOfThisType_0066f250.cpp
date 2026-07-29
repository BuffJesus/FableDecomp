extern "C" long __fastcall
CInputTypeXboxPadButtonEvent_IsEventOfThisType_0066f250_GetKind(
    void* event, void*);
extern "C" long __fastcall
CInputTypeXboxPadButtonEvent_IsEventOfThisType_0066f250_GetButton(
    void* event, void*);

extern "C" __declspec(naked) unsigned char __fastcall
CInputTypeXboxPadButtonEvent_IsEventOfThisType_0066f250(
    void* input_type, void*, const void* event)
{
    __asm
    {
        push esi
        mov esi, [esp + 8]
        push edi
        mov edi, ecx
        mov ecx, esi
        call CInputTypeXboxPadButtonEvent_IsEventOfThisType_0066f250_GetKind
        cmp eax, 13h
        je compare_button
        mov ecx, esi
        call CInputTypeXboxPadButtonEvent_IsEventOfThisType_0066f250_GetKind
        cmp eax, 14h
        je compare_button
        mov ecx, esi
        call CInputTypeXboxPadButtonEvent_IsEventOfThisType_0066f250_GetKind
        cmp eax, 15h
        je compare_button
        pop edi
        xor al, al
        pop esi
        ret 4

    compare_button:
        mov ecx, esi
        call CInputTypeXboxPadButtonEvent_IsEventOfThisType_0066f250_GetButton
        sub eax, [edi + 4]
        neg eax
        sbb eax, eax
        pop edi
        inc eax
        pop esi
        ret 4
    }
}
