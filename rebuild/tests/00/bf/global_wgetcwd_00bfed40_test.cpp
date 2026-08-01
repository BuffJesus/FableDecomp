#include <stdio.h>

typedef unsigned short wchar_t_;

extern "C" wchar_t_ * __cdecl _wgetcwd_stub_impl(wchar_t_ *buffer, int maxlen)
{
    if (buffer && maxlen > 0) {
        buffer[0] = (wchar_t_)'Q';
        buffer[1] = 0;
    }
    return buffer;
}

typedef wchar_t_ * (__cdecl *PFN_wgetcwd_t)(wchar_t_ *, int);
extern "C" PFN_wgetcwd_t __imp__wgetcwd_test = _wgetcwd_stub_impl;

extern "C" wchar_t_ * __cdecl _wgetcwd_test(wchar_t_ *buffer, int maxlen)
{
    return __imp__wgetcwd_test(buffer, maxlen);
}

int main()
{
    wchar_t_ buf[8];
    wchar_t_ *r = _wgetcwd_test(buf, 8);
    if (r == buf && buf[0] == (wchar_t_)'Q') {
        printf("WGETCWD_OK\n");
    } else {
        printf("WGETCWD_FAIL\n");
    }
    return 0;
}