// Self-contained behavior test for Fill_n_0047b630.
#include <stdio.h>

struct Str {
    void* _p;
    Str& operator=(const Str& _rhs);
};

struct Elem {
    unsigned long _a;
    Str           _b;
    unsigned char _c;
};

// Stub definition of the masked __fastcall member copy so the link resolves
// and behavior is exercised.
Str& Str::operator=(const Str& _rhs)
{
    this->_p = _rhs._p;
    return *this;
}

extern "C" Elem* __fastcall
Fill_n_0047b630(Elem* _dest, unsigned int _count, const Elem* _value, void* _unused);

int main()
{
    int mk = 0;
    void* mkp = &mk;

    Elem val;
    val._a = 0x11223344ul;
    val._b._p = mkp;
    val._c = (unsigned char)0xABu;

    Elem buf[5];
    int i;
    for (i = 0; i < 5; ++i) { buf[i]._a = 0; buf[i]._b._p = 0; buf[i]._c = 0; }

    Elem* end = Fill_n_0047b630(buf, 5u, &val, 0);

    int ok = 1;
    if (end != buf + 5) ok = 0;
    for (i = 0; i < 5; ++i)
    {
        if (buf[i]._a != 0x11223344ul) ok = 0;
        if (buf[i]._b._p != mkp) ok = 0;
        if (buf[i]._c != (unsigned char)0xABu) ok = 0;
    }

    // count == 0 must be a no-op returning dest unchanged.
    Elem one; one._a = 7; one._b._p = 0; one._c = 1;
    Elem* end0 = Fill_n_0047b630(&one, 0u, &val, 0);
    if (end0 != &one) ok = 0;
    if (one._a != 7ul) ok = 0;

    if (ok) printf("FILL_N_ELEM12_0047B630_TEST PASS\n");
    else    printf("FILL_N_ELEM12_0047B630_TEST FAIL\n");
    return ok ? 0 : 1;
}