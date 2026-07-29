extern "C" long __fastcall
CInputTypeMouseMovementEvent_IsEventOfThisType_0066f420_GetKind(
    void* event, void*);

extern "C" __declspec(naked) unsigned char __fastcall
CInputTypeMouseMovementEvent_IsEventOfThisType_0066f420(
    void* input_type, void*, const void* event)
{
    __asm
    {
        mov ecx, [esp + 4]
        call CInputTypeMouseMovementEvent_IsEventOfThisType_0066f420_GetKind
        sub eax, 0dh
        neg eax
        sbb eax, eax
        inc eax
        ret 4
    }
}
