#include <stdio.h>

struct Elem {
    long  first;    // +0x00
    char  _pad[60]; // +0x04 .. +0x3f
};                  // sizeof == 0x40

struct CArrayElem {
    Elem* _First;
    Elem* _Last;

    Elem* begin() const { return _First; }
    Elem* end() const   { return _Last; }
    unsigned int size() const { return (unsigned int)(_Last - _First); }

    void erase(Elem* _F, Elem* _L);
    void _Insert_n(Elem* _Where, unsigned int _Count, const Elem& _Val);

    void resize(unsigned int _Newsize, const Elem& _Val);
};

// Stub definitions of the masked members so the link resolves and the two
// dispatch branches are exercised.  These record which branch ran and with
// what arguments, without doing real reallocation.
static int   g_eraseCalled;
static long  g_eraseFromIdx;   // begin()+_Newsize as an index
static long  g_eraseCount;     // end()-first, number erased

static int          g_insertCalled;
static unsigned int g_insertCount;
static long         g_insertValFirst;

void CArrayElem::erase(Elem* _F, Elem* _L)
{
    g_eraseCalled  = 1;
    g_eraseFromIdx = (long)(_F - _First);
    g_eraseCount   = (long)(_L - _F);
}

void CArrayElem::_Insert_n(Elem* _Where, unsigned int _Count, const Elem& _Val)
{
    g_insertCalled   = 1;
    g_insertCount    = _Count;
    g_insertValFirst = _Val.first;
    if (_Where != _Last) g_insertCalled = 2;  // _Where should be end()
}

// The container resize under test, compiled at /Od here for behavior only.
void CArrayElem::resize(unsigned int _Newsize, const Elem& _Val)
{
    if (_Newsize < size())
        erase(begin() + _Newsize, end());
    else
        _Insert_n(end(), _Newsize - size(), _Val);
}

int main()
{
    Elem buf[8];
    int i;
    for (i = 0; i < 8; ++i) { buf[i].first = i; }

    Elem val;
    val.first = 0x1111;

    int ok = 1;

    // Case 1: newsize (7) >= size (5) -> INSERT branch, count = 2.
    CArrayElem a;
    a._First = &buf[0];
    a._Last  = &buf[5];   // size == 5
    g_eraseCalled = g_insertCalled = 0;
    g_insertCount = 0;
    a.resize(7u, val);
    if (g_insertCalled != 1) ok = 0;            // ran and _Where==end()
    if (g_eraseCalled != 0) ok = 0;
    if (g_insertCount != 2u) ok = 0;            // 7 - 5
    if (g_insertValFirst != 0x1111) ok = 0;

    // Case 2: newsize (3) < size (5) -> ERASE branch, from idx 3, count 2.
    CArrayElem b;
    b._First = &buf[0];
    b._Last  = &buf[5];   // size == 5
    g_eraseCalled = g_insertCalled = 0;
    b.resize(3u, val);
    if (g_eraseCalled != 1) ok = 0;
    if (g_insertCalled != 0) ok = 0;
    if (g_eraseFromIdx != 3) ok = 0;            // begin()+3
    if (g_eraseCount != 2) ok = 0;              // end() - (begin()+3)

    // Case 3: newsize == size (5) -> INSERT branch (jae), count 0.
    CArrayElem c;
    c._First = &buf[0];
    c._Last  = &buf[5];
    g_eraseCalled = g_insertCalled = 0;
    g_insertCount = 999u;
    c.resize(5u, val);
    if (g_insertCalled != 1) ok = 0;
    if (g_eraseCalled != 0) ok = 0;
    if (g_insertCount != 0u) ok = 0;

    if (ok) printf("CARRAY_PAIR_RESIZE_00472B9D_TEST PASS\n");
    else    printf("CARRAY_PAIR_RESIZE_00472B9D_TEST FAIL\n");
    return ok ? 0 : 1;
}