#include <cstdio>

// Behavior test for CArray<T>::resize @ 0x00476acf (element stride 0x4c == 76).
//
// Self-contained: the two masked callees (erase / _Insert_n) are defined here as
// real member functions that record their arguments, so we can assert the
// retail dispatch:
//   _Newsize <  size() -> erase(begin() + _Newsize, end())
//   _Newsize >= size() -> _Insert_n(end(), _Newsize - size(), _Val)
// Because Elem is 0x4c bytes, begin()+_Newsize and _Last-_First naturally use
// the 76-byte stride, matching the retail imul/idiv by 0x4c.

struct Elem {
    char raw[0x4c];   // sizeof == 76
};

struct CArrayPair {
    Elem* _First;  // +0x00
    Elem* _Last;   // +0x04

    Elem* begin() const { return _First; }
    Elem* end() const   { return _Last; }
    unsigned int size() const { return (unsigned int)(_Last - _First); }

    void erase(Elem* _F, Elem* _L);
    void _Insert_n(Elem* _Where, unsigned int _Count, const Elem& _Val);

    void resize(unsigned int _Newsize, const Elem& _Val);
};

void CArrayPair::resize(unsigned int _Newsize, const Elem& _Val)
{
    if (_Newsize < size())
        erase(begin() + _Newsize, end());
    else
        _Insert_n(end(), _Newsize - size(), _Val);
}

static int   g_EraseHits;
static int   g_InsertHits;
static Elem* g_EraseF;
static Elem* g_EraseL;
static Elem* g_InsertWhere;
static unsigned int g_InsertCount;
static const Elem* g_InsertVal;

void CArrayPair::erase(Elem* _F, Elem* _L)
{
    ++g_EraseHits;
    g_EraseF = _F;
    g_EraseL = _L;
}

void CArrayPair::_Insert_n(Elem* _Where, unsigned int _Count, const Elem& _Val)
{
    ++g_InsertHits;
    g_InsertWhere = _Where;
    g_InsertCount = _Count;
    g_InsertVal   = &_Val;
}

static void Reset()
{
    g_EraseHits = 0; g_InsertHits = 0;
    g_EraseF = 0; g_EraseL = 0;
    g_InsertWhere = 0; g_InsertCount = 0; g_InsertVal = 0;
}

int main()
{
    static Elem storage[8];
    Elem val;

    CArrayPair arr;
    arr._First = &storage[0];
    arr._Last  = &storage[3];   // size() == 3

    int failures = 0;

    // ERASE: _Newsize < size(). _Newsize=1, size=3 ->
    //   erase(begin()+1, end()).
    Reset();
    arr.resize(1u, val);
    if (g_EraseHits != 1 || g_InsertHits != 0 ||
        g_EraseF != &storage[1] || g_EraseL != &storage[3])
        ++failures;

    // INSERT (grow): _Newsize > size(). _Newsize=5, size=3 ->
    //   _Insert_n(end(), 2, val).
    Reset();
    arr.resize(5u, val);
    if (g_InsertHits != 1 || g_EraseHits != 0 ||
        g_InsertWhere != &storage[3] || g_InsertCount != 2u ||
        g_InsertVal != &val)
        ++failures;

    // BOUNDARY: _Newsize == size() -> jae taken -> INSERT with count 0.
    Reset();
    arr.resize(3u, val);
    if (g_InsertHits != 1 || g_EraseHits != 0 ||
        g_InsertWhere != &storage[3] || g_InsertCount != 0u ||
        g_InsertVal != &val)
        ++failures;

    if (failures != 0) {
        std::printf("CARRAY_PAIR_UL_PUSH_BACK_00476ACF_TEST FAIL count=%d e=%d i=%d\n",
                    failures, g_EraseHits, g_InsertHits);
        return 1;
    }

    std::printf("CARRAY_PAIR_UL_PUSH_BACK_00476ACF_TEST PASS\n");
    return 0;
}