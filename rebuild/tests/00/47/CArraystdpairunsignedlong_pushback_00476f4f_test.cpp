#include <cstdio>

// Behavior test for CArray<std::pair<unsigned_long,...> >::resize @ 0x00476f4f.
// Stride is 0xc4 (196 bytes).  We provide extern stub definitions for the two
// masked member callees (erase / _Insert_n) that record their arguments, then
// drive the dispatch through the three branches: _Newsize < size (ERASE),
// _Newsize == size (INSERT, count 0), _Newsize > size (INSERT, count > 0).

struct Pair196 {
    unsigned long first;
    unsigned long pad[48];
};

struct CArrayPair196 {
    Pair196* _First;
    Pair196* _Last;

    Pair196* begin() const { return _First; }
    Pair196* end() const   { return _Last; }
    unsigned int size() const { return (unsigned int)(_Last - _First); }

    void erase(Pair196* _F, Pair196* _L);
    void _Insert_n(Pair196* _Where, unsigned int _Count, const Pair196& _Val);

    void resize(unsigned int _Newsize, const Pair196& _Val);
};

void CArrayPair196::resize(unsigned int _Newsize, const Pair196& _Val)
{
    if (_Newsize < size())
        erase(begin() + _Newsize, end());
    else
        _Insert_n(end(), _Newsize - size(), _Val);
}

static int      g_EraseCalls;
static Pair196* g_EraseF;
static Pair196* g_EraseL;

static int          g_InsertCalls;
static Pair196*     g_InsertWhere;
static unsigned int g_InsertCount;
static const Pair196* g_InsertVal;

void CArrayPair196::erase(Pair196* _F, Pair196* _L)
{
    ++g_EraseCalls;
    g_EraseF = _F;
    g_EraseL = _L;
}

void CArrayPair196::_Insert_n(Pair196* _Where, unsigned int _Count, const Pair196& _Val)
{
    ++g_InsertCalls;
    g_InsertWhere = _Where;
    g_InsertCount = _Count;
    g_InsertVal = &_Val;
}

static void Reset()
{
    g_EraseCalls = 0; g_EraseF = 0; g_EraseL = 0;
    g_InsertCalls = 0; g_InsertWhere = 0; g_InsertCount = 0; g_InsertVal = 0;
}

int main()
{
    int failures = 0;

    static Pair196 storage[8];
    Pair196 val;
    val.first = 0x5151u;

    CArrayPair196 a;
    a._First = &storage[0];
    a._Last  = &storage[4];   // size() == 4

    // _Newsize (2) < size (4) -> ERASE(begin()+2, end()).
    Reset();
    a.resize(2u, val);
    if (g_EraseCalls != 1 || g_InsertCalls != 0 ||
        g_EraseF != &storage[2] || g_EraseL != &storage[4]) {
        std::printf("erase-path failed F=%p L=%p e=%d i=%d\n",
                    (void*)g_EraseF, (void*)g_EraseL, g_EraseCalls, g_InsertCalls);
        ++failures;
    }

    // _Newsize (4) == size (4) -> INSERT(end(), 0, val).
    Reset();
    a.resize(4u, val);
    if (g_InsertCalls != 1 || g_EraseCalls != 0 ||
        g_InsertWhere != &storage[4] || g_InsertCount != 0u ||
        g_InsertVal != &val) {
        std::printf("insert-eq failed W=%p c=%u v=%p e=%d i=%d\n",
                    (void*)g_InsertWhere, g_InsertCount, (void*)g_InsertVal,
                    g_EraseCalls, g_InsertCalls);
        ++failures;
    }

    // _Newsize (7) > size (4) -> INSERT(end(), 3, val).
    Reset();
    a.resize(7u, val);
    if (g_InsertCalls != 1 || g_EraseCalls != 0 ||
        g_InsertWhere != &storage[4] || g_InsertCount != 3u ||
        g_InsertVal != &val) {
        std::printf("insert-gt failed W=%p c=%u v=%p e=%d i=%d\n",
                    (void*)g_InsertWhere, g_InsertCount, (void*)g_InsertVal,
                    g_EraseCalls, g_InsertCalls);
        ++failures;
    }

    if (failures != 0) {
        std::printf("CARRAY_PUSH_BACK_00476F4F_TEST FAIL count=%d\n", failures);
        return 1;
    }

    std::printf("CARRAY_PUSH_BACK_00476F4F_TEST PASS\n");
    return 0;
}