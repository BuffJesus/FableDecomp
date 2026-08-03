// Byte-exact reconstruction of retail 0x0047b630
// _Fill_n<unsigned_long*, unsigned_int, unsigned_long>
//
// Retail is MSVC7.1 std::_Fill_n inlined over a 12-byte value type whose
// middle member is a small class copied by an out-of-line __fastcall op.
// Element layout inferred from the disasm:
//   [+0] unsigned long   (mov eax,[edi]; mov [esi],eax)
//   [+4] Str  (lea ecx,[edi+4]; push ecx; lea ecx,[esi+4]; call <copy>)
//   [+8] unsigned char   (mov dl,[edi+8]; mov [esi+8],dl)
// sizeof == 0x0c, stride add esi,0x0c.
//
// call/abs operands are relocation-masked in parity, so Str::operator=
// is declared extern (out-of-line) to force the __fastcall call.

struct Str {
    void* _p;
    Str& operator=(const Str& _rhs);   // out-of-line -> __fastcall call
};

struct Elem {
    unsigned long _a;   // +0
    Str           _b;   // +4
    unsigned char _c;   // +8
};                      // sizeof == 12

// __fastcall: this-dest in ecx, count in edx; value + one unused slot on the
// stack (ret 8). Returns the one-past-the-end destination pointer (esi).
extern "C" Elem* __fastcall
Fill_n_0047b630(Elem* _dest, unsigned int _count, const Elem* _value, void* /*unused*/)
{
    // count > 0 (unsigned) makes MSVC emit `test edx,edx; jbe` on the entry
    // guard (retail byte 0x5 is 76=jbe, not 74=je that `!= 0` produced).
    for (; _count > 0; --_count)
    {
        if (_dest)
        {
            _dest->_a = _value->_a;
            _dest->_b = _value->_b;   // Str::operator= -> __fastcall call
            _dest->_c = _value->_c;
        }
        ++_dest;
    }
    return _dest;
}