extern "C" __declspec(naked) void* __fastcall
CInputTypeXboxPadButtonEvent_GetControlDirection_0066f730(
    void* input_type, void*, void* result, const void* event)
{
    __asm
    {
        mov eax, [esp + 4]
        mov dword ptr [eax], 0
        mov dword ptr [eax + 4], 0
        ret 8
    }
}
