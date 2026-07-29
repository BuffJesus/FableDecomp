extern "C" __declspec(naked) void* __fastcall
CInputTypeKeyboardKeyEvent_GetControlDirection_0066f610(
    void* input_type, void*, void* result, const void* event)
{
    __asm
    {
        mov edx, [ecx + 8]
        mov eax, [esp + 4]
        mov [eax], edx
        mov ecx, [ecx + 0ch]
        mov [eax + 4], ecx
        ret 8
    }
}
