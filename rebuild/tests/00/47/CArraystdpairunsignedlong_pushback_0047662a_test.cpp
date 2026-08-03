#include <cstdio>

// Self-contained behaviour test for CArray<std::pair<unsigned_long,...> >::resize
// (manifest "push_back") @ 0x0047662a.  Compiles at /Od, links standalone.
//
// This is a GENUINE-C++ test: it defines the same resize() dispatch as the parity
// source and stubs the two out-of-line container members (erase / _Insert_n) so
// the standalone link resolves and both branches can be observed.  Element size
// is 0x5c (92) bytes; size() == (_Last - _First).

struct Pair {
    unsigned long first;        // +0x00
    char          second[0x58]; // +0x04 .. 0x5b  -> sizeof == 0x5c (92)
};

struct CArrayPair {
    Pair* _First;  // +0x00
    Pair* _Last;   // +0x04

    Pair* begin() const { return _First; }
    Pair* end() const   { return _Last; }
    unsigned int size() const { return (unsigned int)(_Last - _First); }

    void erase(Pair* _F, Pair* _L);
    void _Insert_n(Pair* _Where, unsigned int _Count, const Pair& _Val);

    void resize(unsigned int _Newsize, const Pair& _Val);
};

void CArrayPair::resize(unsigned int _Newsize, const Pair& _Val)
{
    if (_Newsize < size())
        erase(begin() + _Newsize, end());
    else
        _Insert_n(end(), _Newsize - size(), _Val);
}

static int          g_EraseCalls;
static const Pair*  g_EraseF;
static const Pair*  g_EraseL;

static int          g_InsertCalls;
static const Pair*  g_InsertWhere;
static unsigned int g_InsertCount;
static const Pair*  g_InsertVal;

void CArrayPair::erase(Pair* _F, Pair* _L)
{
    ++g_EraseCalls;
    g_EraseF = _F;
    g_EraseL = _L;
}

void CArrayPair::_Insert_n(Pair* _Where, unsigned int _Count, const Pair& _Val)
{
    ++g_InsertCalls;
    g_InsertWhere = _Where;
    g_InsertCount = _Count;
    g_InsertVal   = &_Val;
}

static void Reset()
{
    g_EraseCalls = 0;  g_EraseF = 0;  g_EraseL = 0;
    g_InsertCalls = 0; g_InsertWhere = 0; g_InsertCount = 0; g_InsertVal = 0;
}

int main()
{
    int failures = 0;

    static Pair storage[4];
    Pair valueSentinel;
    valueSentinel.first = 0x5151u;

    CArrayPair arr;
    arr._First = &storage[0];
    arr._Last  = &storage[4];   // size() == 4

    // _Newsize 2 < size(4) -> ERASE(begin()+2, end()).
    Reset();
    arr.resize(2u, valueSentinel);
    if (g_EraseCalls != 1 ||
        g_InsertCalls != 0 ||
        g_EraseF != (arr._First + 2) ||
        g_EraseL != arr._Last)
    {
        std::printf("erase-path failed f=%p l=%p e=%d i=%d\n",
                    (void*)g_EraseF, (void*)g_EraseL, g_EraseCalls, g_InsertCalls);
        ++failures;
    }

    // _Newsize 4 == size(4) -> INSERT(end(), 0, &val)   (jae boundary).
    Reset();
    arr.resize(4u, valueSentinel);
    if (g_InsertCalls != 1 ||
        g_EraseCalls != 0 ||
        g_InsertWhere != arr._Last ||
        g_InsertCount != 0u ||
        g_InsertVal != &valueSentinel)
    {
        std::printf("insert-eq failed w=%p c=%u v=%p e=%d i=%d\n",
                    (void*)g_InsertWhere, g_InsertCount, (void*)g_InsertVal,
                    g_EraseCalls, g_InsertCalls);
        ++failures;
    }

    // _Newsize 7 > size(4) -> INSERT(end(), 3, &val).
    Reset();
    arr.resize(7u, valueSentinel);
    if (g_InsertCalls != 1 ||
        g_EraseCalls != 0 ||
        g_InsertWhere != arr._Last ||
        g_InsertCount != 3u ||
        g_InsertVal != &valueSentinel)
    {
        std::printf("insert-gt failed w=%p c=%u v=%p e=%d i=%d\n",
                    (void*)g_InsertWhere, g_InsertCount, (void*)g_InsertVal,
                    g_EraseCalls, g_InsertCalls);
        ++failures;
    }

    if (failures != 0)
    {
        std::printf("CARRAY_PUSH_BACK_0047662A_TEST FAIL count=%d\n", failures);
        return 1;
    }

    std::printf("CARRAY_PUSH_BACK_0047662A_TEST PASS\n");
    return 0;
}