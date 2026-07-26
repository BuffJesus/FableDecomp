#include <stdlib.h>

extern "C" _onexit_t __cdecl __dllonexit(
    _onexit_t function, _onexit_t** first, _onexit_t** last);
extern "C" _onexit_t* DAT_0143e990;
extern "C" _onexit_t* DAT_0143e9a0;

extern "C" void __cdecl __onexit(_onexit_t function)
{
    if (DAT_0143e9a0 == reinterpret_cast<_onexit_t*>(-1))
    {
        _onexit(function);
        return;
    }
    __dllonexit(function, &DAT_0143e9a0, &DAT_0143e990);
}
