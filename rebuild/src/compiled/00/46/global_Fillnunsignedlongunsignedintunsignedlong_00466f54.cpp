#pragma optimize("s",on)
#include <string.h>

// _Fill_n<unsigned long*, unsigned int, unsigned long> @ retail 0x00466f54 (32 bytes).
// __fastcall(ecx=dest, edx=count, [esp+4]=src ptr), ret 4.
// Size-optimised STL fill_n of a 12-byte value: per-iteration copies the 12-byte
// value out with movsd x3, advances dest by 0xc, decrements count. Returns the
// advanced output iterator (kept in eax), matching STL fill_n semantics.

struct V12 { unsigned long a, b, c; };

V12* __fastcall _Fill_n_v12(V12* _First, unsigned int _Count, const V12* _Val)
{
    for (; 0 < _Count; --_Count, ++_First)
    {
        if (_First)
            memcpy(_First, _Val, 12);
    }
    return _First;
}