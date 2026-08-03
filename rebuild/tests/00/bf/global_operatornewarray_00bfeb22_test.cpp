#include <cstdio>
static unsigned char g_buf[256];
extern "C" void* __cdecl real_impl(unsigned int size)
{
    return size <= sizeof(g_buf) ? (void*)g_buf : (void*)0;
}
extern "C" void* (__cdecl *__imp_operator_new_array)(unsigned int size) = real_impl;
extern "C" void* __cdecl operator_new_array(unsigned int size)
{
    return __imp_operator_new_array(size);
}
int main()
{
    void* p = operator_new_array(16);
    void* r = operator_new_array(9999);
    if (p == (void*)g_buf && r == 0) {
        printf("OAB_OK\n");
    }
    return 0;
}