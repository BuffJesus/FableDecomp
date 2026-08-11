// Faithful reconstruction of std::_Cons_val<...> at retail 0x006fb1a0.
// Constructs a pair<CWideString,float> at *_P from source *_V:
//   - constructs the CWideString member (offset 0) via the string _Cons_val (0x99ec30)
//   - copies the float member (offset +4)
// ecx = _P (dest pair, == &_P->first), edx = _V (src pair).

// Dest string wrapper: its ctor-helper is __fastcall (ecx = &_P->first == _P),
// taking the source string as a single pushed stack argument.
struct WStr {
    void* p;
    void Cons(void* srcStr);   // retail 0x99ec30
};

struct PairWF {
    WStr  first;    // offset 0
    float second;   // offset 4
};

void __fastcall _Cons_val(PairWF* _P, void* _Vraw)
{
    PairWF* _V = (PairWF*)_Vraw;
    if (_P != 0)
    {
        _P->first.Cons(&_V->first);
        _P->second = _V->second;
    }
}