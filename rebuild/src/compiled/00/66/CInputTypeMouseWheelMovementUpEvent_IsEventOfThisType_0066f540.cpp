extern "C" long __fastcall
CInputTypeMouseWheelMovementUpEvent_IsEventOfThisType_0066f540_GetKind(
    void* event, void*);
extern "C" float __fastcall
CInputTypeMouseWheelMovementUpEvent_IsEventOfThisType_0066f540_GetDelta(
    void* event, void*);
extern "C" float
CInputTypeMouseWheelMovementUpEvent_IsEventOfThisType_0066f540_Threshold;

extern "C" __declspec(naked) unsigned char __fastcall
CInputTypeMouseWheelMovementUpEvent_IsEventOfThisType_0066f540(
    void* input_type, void*, const void* event)
{
    __asm
    {
        push esi
        mov esi, [esp + 8]
        mov ecx, esi
        call CInputTypeMouseWheelMovementUpEvent_IsEventOfThisType_0066f540_GetKind
        cmp eax, 0eh
        jne not_up
        mov ecx, esi
        call CInputTypeMouseWheelMovementUpEvent_IsEventOfThisType_0066f540_GetDelta
        fcomp dword ptr [CInputTypeMouseWheelMovementUpEvent_IsEventOfThisType_0066f540_Threshold]
        fnstsw ax
        test ah, 41h
        jne not_up
        mov al, 1
        pop esi
        ret 4

    not_up:
        xor al, al
        pop esi
        ret 4
    }
}
