extern "C" long __fastcall
CInputTypeMouseWheelMovementEvent_IsEventOfThisType_0066f4b0_GetKind(
    void* event, void*);

extern "C" __declspec(naked) unsigned char __fastcall
CInputTypeMouseWheelMovementEvent_IsEventOfThisType_0066f4b0(
    void* input_type, void*, const void* event)
{
    __asm
    {
        mov ecx, [esp + 4]
        call CInputTypeMouseWheelMovementEvent_IsEventOfThisType_0066f4b0_GetKind
        sub eax, 0eh
        neg eax
        sbb eax, eax
        inc eax
        ret 4
    }
}
