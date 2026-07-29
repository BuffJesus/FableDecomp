extern "C" long __fastcall
CInputTypeKeyboardKeyEvent_IsEventOfThisType_0066ee20_GetKind(
    void* event, void*);
extern "C" long __fastcall
CInputTypeKeyboardKeyEvent_IsEventOfThisType_0066ee20_GetKey(
    void* event, void*);

extern "C" __declspec(naked) unsigned char __fastcall
CInputTypeKeyboardKeyEvent_IsEventOfThisType_0066ee20(
    void* input_type, void*, const void* event)
{
    __asm
    {
        push esi
        mov esi, [esp + 8]
        push edi
        mov edi, ecx
        mov ecx, esi
        call CInputTypeKeyboardKeyEvent_IsEventOfThisType_0066ee20_GetKind
        cmp eax, 1
        je compare_key
        mov ecx, esi
        call CInputTypeKeyboardKeyEvent_IsEventOfThisType_0066ee20_GetKind
        cmp eax, 2
        je compare_key
        mov ecx, esi
        call CInputTypeKeyboardKeyEvent_IsEventOfThisType_0066ee20_GetKind
        cmp eax, 3
        je compare_key
        pop edi
        xor al, al
        pop esi
        ret 4

    compare_key:
        mov ecx, esi
        call CInputTypeKeyboardKeyEvent_IsEventOfThisType_0066ee20_GetKey
        sub eax, [edi + 4]
        neg eax
        sbb eax, eax
        pop edi
        inc eax
        pop esi
        ret 4
    }
}
