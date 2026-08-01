#include <cstdio>

// Standalone functional test of the genuine-C++ resize() dispatch for
// 0x004799c6.  Mirrors the parity source's types and provides in-test
// definitions of the two out-of-line members (erase / _Insert_n) so test.obj
// links on its own.  Element stride is 0x54 (84) bytes.

struct Pair {
    unsigned long first;       // +0x00
    char          second[80];  // +0x04 .. +0x53
};                             // sizeof == 0x54 (84)

struct CArrayPair {
    Pair* _First;   // +0x00
    Pair* _Last;    // +0x04

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

// ---- instrumentation --------------------------------------------------------
static int    g_EraseCalls;
static Pair*  g_EraseF;
static Pair*  g_EraseL;
static int    g_InsertCalls;
static Pair*  g_InsertWhere;
static unsigned int g_InsertCount;
static const Pair* g_InsertVal;

void CArrayPair::erase(Pair* _F, Pair* _L)
{
    ++g_EraseCalls; g_EraseF = _F; g_EraseL = _L;
}
void CArrayPair::_Insert_n(Pair* _Where, unsigned int _Count, const Pair& _Val)
{
    ++g_InsertCalls; g_InsertWhere = _Where; g_InsertCount = _Count; g_InsertVal = &_Val;
}

static void Reset()
{
    g_EraseCalls = 0; g_EraseF = 0; g_EraseL = 0;
    g_InsertCalls = 0; g_InsertWhere = 0; g_InsertCount = 0; g_InsertVal = 0;
}

int main()
{
    int failures = 0;
    static Pair storage[8];
    Pair value;
    value.first = 0x5151;

    CArrayPair a;
    a._First = &storage[0];
    a._Last  = &storage[4];          // size() = 4

    // _Newsize < size()  -> erase(begin()+_Newsize, end())
    Reset();
    a.resize(2, value);
    if (g_EraseCalls != 1 || g_InsertCalls != 0 ||
        g_EraseF != &storage[2] || g_EraseL != &storage[4]) {
        std::printf("erase-path failed F=%p L=%p e=%d i=%d\n",
                    (void*)g_EraseF, (void*)g_EraseL, g_EraseCalls, g_InsertCalls);
        ++failures;
    }

    // _Newsize == size()  -> insert path, count = 0
    Reset();
    a.resize(4, value);
    if (g_InsertCalls != 1 || g_EraseCalls != 0 ||
        g_InsertWhere != &storage[4] || g_InsertCount != 0u ||
        g_InsertVal != &value) {
        std::printf("insert-eq failed W=%p c=%u v=%p e=%d i=%d\n",
                    (void*)g_InsertWhere, g_InsertCount, (void*)g_InsertVal,
                    g_EraseCalls, g_InsertCalls);
        ++failures;
    }

    // _Newsize > size()  -> insert path, count = _Newsize - size() = 3
    Reset();
    a.resize(7, value);
    if (g_InsertCalls != 1 || g_EraseCalls != 0 ||
        g_InsertWhere != &storage[4] || g_InsertCount != 3u ||
        g_InsertVal != &value) {
        std::printf("insert-gt failed W=%p c=%u v=%p e=%d i=%d\n",
                    (void*)g_InsertWhere, g_InsertCount, (void*)g_InsertVal,
                    g_EraseCalls, g_InsertCalls);
        ++failures;
    }

    if (failures != 0) {
        std::printf("CARRAY_PUSH_BACK_004799C6_TEST FAIL count=%d\n", failures);
        return 1;
    }
    std::printf("CARRAY_PUSH_BACK_004799C6_TEST PASS\n");
    return 0;
}