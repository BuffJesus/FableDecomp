extern "C" long __fastcall
CInputTypeXboxPadRightStickEvent_IsEventOfThisType_0066f390_GetKind(
    void* event, void*);

extern "C" __declspec(naked) unsigned char __fastcall
CInputTypeXboxPadRightStickEvent_IsEventOfThisType_0066f390(
    void* input_type, void*, const void* event)
{
    __asm
    {
        mov ecx, [esp + 4]
        call CInputTypeXboxPadRightStickEvent_IsEventOfThisType_0066f390_GetKind
        sub eax, 12h
        neg eax
        sbb eax, eax
        inc eax
        ret 4
    }
}
