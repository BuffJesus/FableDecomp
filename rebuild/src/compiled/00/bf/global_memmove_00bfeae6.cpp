extern "C" void* __imp_memmove;

extern "C" __declspec(naked) void* memmove(void* dst, const void* src, size_t count)
{
    __asm
    {
        jmp dword ptr [__imp_memmove]
    }
}