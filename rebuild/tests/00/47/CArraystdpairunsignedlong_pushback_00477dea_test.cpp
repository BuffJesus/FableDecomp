// Self-contained behavior test for the resize/push_back dispatch at 0x00477dea.
//
// We instantiate the same struct shape used in source_cpp (element stride 0x88)
// and provide real definitions for the two out-of-line member callees so the
// program links and runs at /Od.  The callees record their arguments; the test
// drives both branches (index < size -> erase; index >= size -> _Insert_n) and
// verifies the computed slot / grow-count / end pointers, then prints an exact
// uppercase pass token.

#include <cstdio>

struct Pair {
    unsigned long first;
    unsigned char rest[0x84];
};   // sizeof == 0x88

struct CArrayPair {
    Pair* _First;
    Pair* _Last;

    Pair* begin() const { return _First; }
    Pair* end() const   { return _Last; }
    unsigned int size() const { return (unsigned int)(_Last - _First); }

    void erase(Pair* _F, Pair* _L);
    void _Insert_n(Pair* _Where, unsigned int _Count, const Pair& _Val);

    void push_back(unsigned int _Newsize, const Pair& _Val);
};

// ---- recorded callee arguments ---------------------------------------------
static int   g_erase_called = 0;
static Pair* g_erase_first   = 0;
static Pair* g_erase_last    = 0;

static int   g_insert_called = 0;
static Pair* g_insert_where   = 0;
static unsigned int g_insert_count = 0;
static const Pair* g_insert_val = 0;

void CArrayPair::erase(Pair* _F, Pair* _L)
{
    g_erase_called = 1;
    g_erase_first  = _F;
    g_erase_last   = _L;
}

void CArrayPair::_Insert_n(Pair* _Where, unsigned int _Count, const Pair& _Val)
{
    g_insert_called = 1;
    g_insert_where  = _Where;
    g_insert_count  = _Count;
    g_insert_val    = &_Val;
}

void CArrayPair::push_back(unsigned int _Newsize, const Pair& _Val)
{
    if (_Newsize < size())
        erase(begin() + _Newsize, end());
    else
        _Insert_n(end(), _Newsize - size(), _Val);
}

int main()
{
    static Pair buffer[16];
    const unsigned int COUNT = 4;

    CArrayPair a;
    a._First = buffer;
    a._Last  = buffer + COUNT;   // 4 populated elements

    Pair value;
    value.first = 0x1234;

    // --- Case 1: index < size -> erase ------------------------------------
    g_erase_called = g_insert_called = 0;
    unsigned int idx1 = 2;
    a.push_back(idx1, value);

    if (!g_erase_called || g_insert_called) { std::printf("FAIL branch1\n"); return 1; }
    if (g_erase_first != a._First + idx1)   { std::printf("FAIL efirst\n"); return 1; }
    if (g_erase_last  != a._Last)           { std::printf("FAIL elast\n");  return 1; }

    // --- Case 2: index >= size -> _Insert_n -------------------------------
    g_erase_called = g_insert_called = 0;
    unsigned int idx2 = 7;
    a.push_back(idx2, value);

    if (g_erase_called || !g_insert_called) { std::printf("FAIL branch2\n"); return 1; }
    if (g_insert_where != a._Last)          { std::printf("FAIL iwhere\n"); return 1; }
    if (g_insert_count != (idx2 - COUNT))   { std::printf("FAIL icount\n"); return 1; }
    if (g_insert_val   != &value)           { std::printf("FAIL ival\n");   return 1; }

    std::printf("PUSH_BACK_RESIZE_STRIDE88_TEST PASS\n");
    return 0;
}