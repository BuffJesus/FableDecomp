#include <stdlib.h>

extern "C" __declspec(naked) _onexit_t __cdecl __dllonexit(
    _onexit_t, _onexit_t**, _onexit_t**)
{
    __asm {
        _emit 0xff
        _emit 0x25
        _emit 0x74
        _emit 0x01
        _emit 0x44
        _emit 0x01
    }
}
