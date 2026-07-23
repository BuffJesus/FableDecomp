#pragma optimize("s",on)
typedef void (__cdecl *_PVFV)(void);
extern "C" _PVFV __cdecl _onexit_impl(_PVFV func);

extern "C" int __cdecl atexit_impl(_PVFV func)
{
    return (_onexit_impl(func) == 0) ? -1 : 0;
}