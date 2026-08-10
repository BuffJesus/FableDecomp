#include <cstdio>
typedef unsigned int size_type;

static size_type g_last = 0;
static char g_buf[64];

extern "C" void * __cdecl helper_bfeb1c(size_type n)
{
    g_last = n;
    return g_buf;
}

void * __fastcall operator_new(size_type n);

int main()
{
    void *p = operator_new(24u);
    if (p == g_buf && g_last == 24u)
        std::printf("004c73b0_TEST PASS\n");
    else
        std::printf("FAIL p=%p last=%u\n", p, g_last);
    return 0;
}