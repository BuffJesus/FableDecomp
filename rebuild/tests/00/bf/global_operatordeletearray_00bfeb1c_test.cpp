#include <cstdio>
#include <cstdlib>

extern "C" void (__cdecl *__imp_operator_delete_array)(void*) = &free;

extern "C" __declspec(naked) void __cdecl operator_delete_array(void* p)
{
    __asm
    {
        jmp dword ptr [__imp_operator_delete_array]
    }
}

int main()
{
    void* p = malloc(16);
    operator_delete_array(p);
    printf("OK_ARRAY_DELETE_THUNK\n");
    return 0;
}