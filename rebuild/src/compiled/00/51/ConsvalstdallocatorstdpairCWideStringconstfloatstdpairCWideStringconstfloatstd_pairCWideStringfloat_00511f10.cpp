// _Cons_val for std::allocator< std::pair<CWideString const, float> >.
// Retail 0x00511f10.  ecx = destination pair, edx = source pair.
//
// Constructs the pair's `first` (a CWideString) in place from the source pair
// via the CWideString copy-construct helper (reloc call to 0x0099ec30), then
// copies the `second` (float) member.  Guards against a null destination.

struct WStr {
    void* p0;   // opaque CWideString; only its construction matters here
};

struct Pair {
    WStr  first;                 // +0 : CWideString (const key)
    float second;                // +4 : float value
    void  ConsFirst(Pair* src);  // reloc helper 0x0099ec30 (this = dest, arg = src)
};

void __fastcall _Cons_val(Pair* dest, Pair* src)
{
    if (dest)
    {
        dest->ConsFirst(src);
        dest->second = src->second;
    }
}