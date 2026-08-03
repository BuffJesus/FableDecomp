extern "C" void (__cdecl *__imp_operator_delete_array)(void*);

extern "C" __declspec(naked) void __cdecl operator_delete_array(void* p)
{
    __asm
    {
        jmp dword ptr [__imp_operator_delete_array]
    }
}