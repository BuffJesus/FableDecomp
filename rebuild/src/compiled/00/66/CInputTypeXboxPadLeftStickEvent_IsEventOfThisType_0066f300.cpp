extern "C" long __fastcall
CInputTypeXboxPadLeftStickEvent_IsEventOfThisType_0066f300_GetKind(
    void* event, void*);

extern "C" __declspec(naked) unsigned char __fastcall
CInputTypeXboxPadLeftStickEvent_IsEventOfThisType_0066f300(
    void* input_type, void*, const void* event)
{
    __asm
    {
        mov ecx, [esp + 4]
        call CInputTypeXboxPadLeftStickEvent_IsEventOfThisType_0066f300_GetKind
        sub eax, 11h
        neg eax
        sbb eax, eax
        inc eax
        ret 4
    }
}
