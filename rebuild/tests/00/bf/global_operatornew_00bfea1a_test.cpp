#include <cstdio>

typedef void* (__cdecl *FnPtr)(unsigned int);
static void* __cdecl fake_impl(unsigned int size)
{
    return (void*)(size + 0x1000);
}

FnPtr g_slot = fake_impl;

extern "C" void* __cdecl operator_new(unsigned int size)
{
    return g_slot(size);
}

int main()
{
    void* r = operator_new(4);
    if (r == (void*)0x1004)
    {
        printf("OPNEW_THUNK_OK\n");
        return 0;
    }
    printf("OPNEW_THUNK_FAIL %p\n", r);
    return 1;
}