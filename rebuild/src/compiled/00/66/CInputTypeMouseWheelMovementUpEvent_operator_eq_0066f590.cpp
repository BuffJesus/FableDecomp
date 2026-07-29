extern "C" __declspec(naked) unsigned char __fastcall
CInputTypeMouseWheelMovementUpEvent_operator_eq_0066f590(
    void* event, void*, const void* other)
{
    __asm
    {
        push esi
        mov esi, ecx
        mov ecx, [esp + 8]
        mov eax, [ecx]
        push edi
        call dword ptr [eax + 4]
        mov edx, [esi]
        mov ecx, esi
        mov edi, eax
        call dword ptr [edx + 4]
        sub eax, edi
        neg eax
        sbb eax, eax
        pop edi
        inc eax
        pop esi
        ret 4
    }
}
