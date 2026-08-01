#include <stdio.h>

extern "C" void* __imp_memmove;

extern "C" __declspec(naked) void* memmove(void* dst, const void* src, size_t count)
{
    __asm
    {
        jmp dword ptr [__imp_memmove]
    }
}

static char g_dst[16];
static char g_src[16] = "hello_world_ab";

void* __cdecl real_memmove_impl(void* d, const void* s, size_t n)
{
    char* pd = (char*)d;
    const char* ps = (const char*)s;
    for (size_t i = 0; i < n; ++i) pd[i] = ps[i];
    return d;
}

void* __imp_memmove = (void*)real_memmove_impl;

int main()
{
    void* r = memmove(g_dst, g_src, 15);
    if (r == (void*)g_dst && g_dst[0]=='h' && g_dst[13]=='b')
    {
        printf("MEMMOVE_THUNK_OK\n");
    }
    return 0;
}