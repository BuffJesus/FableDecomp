#include <cstdio>

struct PairSrc;

struct CWideString {
    void* buf;
    int   a;
    int   b;
    void CopyCtor(PairSrc* src);
};

struct PairSrc { CWideString first; float second; };
struct PairDst { CWideString first; float second; };

// Local stand-in for retail sub 0x4f9408: copy the CWideString fields from src->first.
void CWideString::CopyCtor(PairSrc* src) {
    this->buf = src->first.buf;
    this->a   = src->first.a + 1;   // observable side-effect proving the ctor ran
    this->b   = src->first.b;
}

void __fastcall _Cons_val(PairDst* dest, PairSrc* src);

int main() {
    PairSrc src;
    src.first.buf = (void*)0x1234; src.first.a = 10; src.first.b = 20;
    src.second = 3.5f;

    char storage[64];
    PairDst* dest = (PairDst*)storage;

    _Cons_val(dest, &src);

    bool ok = (dest->first.a == 11) && (dest->first.b == 20)
              && (dest->first.buf == (void*)0x1234) && (dest->second == 3.5f);

    // null dest must be a no-op (no crash)
    _Cons_val(0, &src);

    if (ok) printf("CONSVAL_OK a=%d b=%d f=%.1f\n", dest->first.a, dest->first.b, dest->second);
    else    printf("CONSVAL_FAIL\n");
    return ok ? 0 : 1;
}