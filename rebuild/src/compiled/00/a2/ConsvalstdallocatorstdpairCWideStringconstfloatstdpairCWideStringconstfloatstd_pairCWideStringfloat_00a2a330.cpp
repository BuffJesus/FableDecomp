// _Cons_val<allocator<pair<CWideString const,float> >, pair<CWideString const,float>, pair<CWideString,float> >
// retail 0x00a2a330
// MSVC7.1 STL allocator::_Cons_val: placement copy-construct a pair<CWideString const,float> at dest from src.
// ecx = dest pair, edx = src pair. Empty allocator elided -> two register (fastcall) pointer params.

struct CCharString {
    char* p;
    // retail 0x99ec30 = CCharString::CCharString(const CCharString&); modeled as a this-in-ecx member
    // taking the source by pointer on the stack.
    void CopyFrom(const CCharString* other);
};

// pair<CWideString const, float>: first = CWideString (CCharString at +0), second = float at +4.
struct PairKV {
    CCharString first;   // +0
    float       second;  // +4
};

void __fastcall _Cons_val(PairKV* dest, PairKV* src)
{
    if (dest) {
        dest->first.CopyFrom(&src->first);
        dest->second = src->second;
    }
}