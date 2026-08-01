extern "C" void (__cdecl* g_FableFreeThunkTarget_00440154)();

extern "C" __declspec(naked) void __cdecl free_00bfea14(void* p)
{
    __asm
    {
        jmp dword ptr [g_FableFreeThunkTarget_00440154]
    }
}