extern "C" float __fastcall
CInputTypeMouseWheelMovementEvent_GetControlDirection_0066f930_GetDelta(
    void* event, void*);

extern "C" __declspec(naked) void* __fastcall
CInputTypeMouseWheelMovementEvent_GetControlDirection_0066f930(
    void* input_type, void*, void* result, const void* event)
{
    __asm
    {
        mov ecx, [esp + 8]
        call CInputTypeMouseWheelMovementEvent_GetControlDirection_0066f930_GetDelta
        mov eax, [esp + 4]
        fstp dword ptr [eax + 4]
        mov dword ptr [eax], 0
        ret 8
    }
}
