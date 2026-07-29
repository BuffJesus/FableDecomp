extern "C" long __fastcall
CInputTypeMouseWheelMovementDownEvent_IsEventOfThisType_0066f5c0_GetKind(
    void* event, void*);
extern "C" float __fastcall
CInputTypeMouseWheelMovementDownEvent_IsEventOfThisType_0066f5c0_GetDelta(
    void* event, void*);
extern "C" float
CInputTypeMouseWheelMovementDownEvent_IsEventOfThisType_0066f5c0_Threshold;

extern "C" __declspec(naked) unsigned char __fastcall
CInputTypeMouseWheelMovementDownEvent_IsEventOfThisType_0066f5c0(
    void* input_type, void*, const void* event)
{
    __asm
    {
        push esi
        mov esi, [esp + 8]
        mov ecx, esi
        call CInputTypeMouseWheelMovementDownEvent_IsEventOfThisType_0066f5c0_GetKind
        cmp eax, 0eh
        jne not_down
        mov ecx, esi
        call CInputTypeMouseWheelMovementDownEvent_IsEventOfThisType_0066f5c0_GetDelta
        fld dword ptr [CInputTypeMouseWheelMovementDownEvent_IsEventOfThisType_0066f5c0_Threshold]
        fchs
        fxch st(1)
        fcompp
        fnstsw ax
        test ah, 5
        jp not_down
        mov al, 1
        pop esi
        ret 4

    not_down:
        xor al, al
        pop esi
        ret 4
    }
}
