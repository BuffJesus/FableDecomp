extern "C" __declspec(naked) void __cdecl __dllonexit()
{
    __asm {
        jmp dword ptr [0x01440174]
    }
}