#include <cstdio>
typedef unsigned int size_t_;
inline void* operator new(size_t_, void* p) { return p; }

struct CWideString {
    void* buf;
    CWideString(const CWideString& o) { buf = o.buf; }
    CWideString() { buf = 0; }
};

struct WPair {
    CWideString first;
    float second;
};

void __fastcall _Cons_val(WPair* _Ptr, WPair* _Val)
{
    if (_Ptr != 0) {
        new (_Ptr) CWideString(_Val->first);
        _Ptr->second = _Val->second;
    }
}

int main() {
    WPair src;
    src.first.buf = (void*)0xDEAD;
    src.second = 3.5f;

    char raw[sizeof(WPair)];
    WPair* dst = (WPair*)raw;
    dst->first.buf = 0;
    dst->second = 0.0f;

    _Cons_val(dst, &src);

    int ok1 = (dst->first.buf == (void*)0xDEAD);
    int ok2 = (dst->second == 3.5f);

    // null dest must be a no-op (no crash)
    _Cons_val(0, &src);

    if (ok1 && ok2) {
        printf("CONS_VAL_OK buf=%p f=%.1f\n", dst->first.buf, dst->second);
        return 0;
    }
    printf("FAIL\n");
    return 1;
}