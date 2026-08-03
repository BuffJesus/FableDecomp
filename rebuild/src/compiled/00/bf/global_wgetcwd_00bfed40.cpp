typedef unsigned short wchar_t_;

extern "C" wchar_t_ * __cdecl _wgetcwd_impl(wchar_t_ *buffer, int maxlen);

typedef wchar_t_ * (__cdecl *PFN_wgetcwd)(wchar_t_ *, int);
extern "C" PFN_wgetcwd __imp__wgetcwd = _wgetcwd_impl;

extern "C" wchar_t_ * __cdecl _wgetcwd(wchar_t_ *buffer, int maxlen)
{
    return __imp__wgetcwd(buffer, maxlen);
}