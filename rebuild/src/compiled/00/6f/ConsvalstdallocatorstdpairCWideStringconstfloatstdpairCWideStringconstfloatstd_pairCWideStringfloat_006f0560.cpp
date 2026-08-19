// _Cons_val for std::pair<CWideString const, float>
// __fastcall: ecx = dest pair*, edx = source pair*
typedef unsigned int size_t_;
inline void* operator new(size_t_, void* p) { return p; }

struct CWideString {
    void* buf;                          // +0
    CWideString(const CWideString& o);  // copy-ctor -> call 0x99ec30 (external)
};

struct WPair {
    CWideString first;   // +0 (one pointer)
    float second;        // +4
};

// Free __fastcall: ecx = _Ptr (dest), edx = _Val (source).
void __fastcall _Cons_val(WPair* _Ptr, WPair* _Val)
{
    if (_Ptr != 0) {
        new (_Ptr) CWideString(_Val->first);
        _Ptr->second = _Val->second;
    }
}