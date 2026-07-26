extern "C" __declspec(naked) void __cdecl initterm()
{
    __asm {
        jmp dword ptr [0x01440190]
    }
}