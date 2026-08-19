#pragma optimize("s",on)
// _Copy_backward<CHeroMarriageInfo*,CHeroMarriageInfo*>  @ retail 0x00485372
// Mangled: ??$_Copy_backward@PAVCHeroMarriageInfo@@PAV1@@std@@YI
//          PAVCHeroMarriageInfo@@PAV1@00U_Nonscalar_ptr_iterator_tag@0@@Z
// __fastcall, returns CHeroMarriageInfo*, 5 params:
//   ecx = _First, edx = _Last, [stack] _Dest, _Dest2, _Nonscalar_ptr_iterator_tag
// Copies [_First,_Last) backward into [..., _Dest); 28-byte (7 dword) elements.
//
// Retail codegen: computes a signed element COUNT = (_Last - _First)/0x1c via
// idiv, then a counted countdown loop. _Last is held in a preserved reg (ebx);
// _Dest is left in its stack home and decremented in place (sub [esp+..],0x1c)
// because the per-element 7-dword copy lowers to `rep movsd`, which clobbers
// edi/esi/ecx each iteration -- so _Dest must be reloaded from a stable slot.
// The counted `for (n = _Last-_First; 0 < n; --n)` spelling is what forces the
// idiv/`test eax,eax; jle` + `dec eax; jne` shape (NOT a `_First != _Last`
// pointer-compare, which enregisters _Dest and diverges).

// 28-byte POD -> compiler-generated assignment lowers to a 7-dword block move.
struct CHeroMarriageInfo
{
    unsigned long m[7];   // 0x1c bytes
};

// Empty tag type, passed by value (occupies one stack slot).
struct _Nonscalar_ptr_iterator_tag {};

CHeroMarriageInfo* __fastcall _Copy_backward(
    CHeroMarriageInfo* _First,
    CHeroMarriageInfo* _Last,
    CHeroMarriageInfo* _Dest,
    CHeroMarriageInfo* _Dest2,
    _Nonscalar_ptr_iterator_tag)
{   // copy [_First, _Last) backwards to [..., _Dest)
    (void)_Dest2;
    for (int _N = (int)(_Last - _First); 0 < _N; --_N)
        *--_Dest = *--_Last;
    return (_Dest);
}