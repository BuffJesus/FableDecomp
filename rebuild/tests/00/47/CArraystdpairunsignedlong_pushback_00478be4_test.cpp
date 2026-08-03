#include <cstdio>

// Behaviour test for the genuine-C++ reconstruction of retail 0x00478be4,
// CArray<std::pair<unsigned_long,...> >::push_back (the classic
// resize(_Newsize, const T&) dispatch, element stride 0x48 = 72 bytes).
//
// Standalone: re-declares the same class layout and resize() body, and DEFINES
// the two callees (erase / _Insert_n) so the /Od link resolves with no external
// symbols.  The recorded arguments prove the dispatch picks the right branch and
// builds the right slot address / fill count.

struct Elem { char _pad[0x48]; };   // sizeof == 72

struct CArrayElem {
    Elem* _First;   // +0x00
    Elem* _Last;    // +0x04

    Elem* begin() const { return _First; }
    Elem* end()   const { return _Last; }
    unsigned int size() const { return (unsigned int)(_Last - _First); }

    void erase(Elem* _F, Elem* _L);
    void _Insert_n(Elem* _Where, unsigned int _Count, const Elem& _Val);

    void resize(unsigned int _Newsize, const Elem& _Val);
};

static int    g_eraseCalls;
static Elem*  g_eraseF;
static Elem*  g_eraseL;

static int    g_insertCalls;
static Elem*  g_insertWhere;
static unsigned int g_insertCount;
static const Elem*  g_insertVal;

void CArrayElem::erase(Elem* _F, Elem* _L)
{
    ++g_eraseCalls;
    g_eraseF = _F;
    g_eraseL = _L;
}

void CArrayElem::_Insert_n(Elem* _Where, unsigned int _Count, const Elem& _Val)
{
    ++g_insertCalls;
    g_insertWhere = _Where;
    g_insertCount = _Count;
    g_insertVal   = &_Val;
}

void CArrayElem::resize(unsigned int _Newsize, const Elem& _Val)
{
    if (_Newsize < size())
        erase(begin() + _Newsize, end());
    else
        _Insert_n(end(), _Newsize - size(), _Val);
}

static void Reset()
{
    g_eraseCalls = 0; g_eraseF = 0; g_eraseL = 0;
    g_insertCalls = 0; g_insertWhere = 0; g_insertCount = 0; g_insertVal = 0;
}

int main()
{
    int failures = 0;

    static Elem storage[4];
    Elem valueSentinel;

    CArrayElem c;
    c._First = &storage[0];
    c._Last  = &storage[4];          // size() == 4

    // _Newsize < size()  -> erase(begin()+_Newsize, end())
    Reset();
    c.resize(2, valueSentinel);
    if (g_eraseCalls != 1 || g_insertCalls != 0 ||
        g_eraseF != &storage[2] || g_eraseL != &storage[4])
    {
        std::printf("erase-path failed F=%p L=%p e=%d i=%d\n",
                    (void*)g_eraseF, (void*)g_eraseL, g_eraseCalls, g_insertCalls);
        ++failures;
    }

    // _Newsize == size() -> _Insert_n(end(), 0, _Val)
    Reset();
    c.resize(4, valueSentinel);
    if (g_insertCalls != 1 || g_eraseCalls != 0 ||
        g_insertWhere != &storage[4] || g_insertCount != 0 ||
        g_insertVal != &valueSentinel)
    {
        std::printf("insert-eq failed W=%p n=%u v=%p e=%d i=%d\n",
                    (void*)g_insertWhere, g_insertCount, (const void*)g_insertVal,
                    g_eraseCalls, g_insertCalls);
        ++failures;
    }

    // _Newsize > size()  -> _Insert_n(end(), _Newsize-size(), _Val)
    Reset();
    c.resize(7, valueSentinel);
    if (g_insertCalls != 1 || g_eraseCalls != 0 ||
        g_insertWhere != &storage[4] || g_insertCount != 3 ||
        g_insertVal != &valueSentinel)
    {
        std::printf("insert-gt failed W=%p n=%u v=%p e=%d i=%d\n",
                    (void*)g_insertWhere, g_insertCount, (const void*)g_insertVal,
                    g_eraseCalls, g_insertCalls);
        ++failures;
    }

    if (failures != 0)
    {
        std::printf("CARRAY_PUSH_BACK_00478BE4_TEST FAIL count=%d\n", failures);
        return 1;
    }

    std::printf("CARRAY_PUSH_BACK_00478BE4_TEST PASS\n");
    return 0;
}