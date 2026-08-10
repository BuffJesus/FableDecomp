
extern "C" __declspec(naked) long __cdecl FtoL(float f)
{
    (void)f;
    __asm
    {
        push ecx
        fld dword ptr [esp+8]
        fistp dword ptr [esp]
        mov eax, dword ptr [esp]
        pop ecx
        ret
    }
}