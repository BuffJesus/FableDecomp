extern "C" void* (__cdecl *__imp_operator_new_array)(unsigned int size);
__declspec(naked) void* __cdecl operator_new_array(unsigned int size)
{
    __asm { jmp dword ptr [__imp_operator_new_array] }
}