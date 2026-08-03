#include <cstdio>

// Self-contained /Od harness proving the resize dispatch logic of 0x004721b3.
// Element stride is 4 bytes (one long).  begin/end are pointers into a backing
// array; size() = (end - begin).  The two out-of-line members are stubbed here
// so the harness can observe which branch fired and with what arguments.

struct Elem { long value; };

static int   g_EraseCalls;
static Elem* g_EraseF;
static Elem* g_EraseL;

static int          g_InsertCalls;
static Elem*        g_InsertWhere;
static unsigned int g_InsertCount;
static const Elem*  g_InsertVal;

struct CArrayElem {
    Elem* _First;
    Elem* _Last;

    Elem* begin() const { return _First; }
    Elem* end() const   { return _Last; }
    unsigned int size() const { return (unsigned int)(_Last - _First); }

    void erase(Elem* _F, Elem* _L)
    {
        ++g_EraseCalls; g_EraseF = _F; g_EraseL = _L;
    }
    void _Insert_n(Elem* _Where, unsigned int _Count, const Elem& _Val)
    {
        ++g_InsertCalls; g_InsertWhere = _Where; g_InsertCount = _Count;
        g_InsertVal = &_Val;
    }

    void resize(unsigned int _Newsize, const Elem& _Val)
    {
        if (_Newsize < size())
            erase(begin() + _Newsize, end());
        else
            _Insert_n(end(), _Newsize - size(), _Val);
    }
};

static void Reset()
{
    g_EraseCalls = 0; g_EraseF = 0; g_EraseL = 0;
    g_InsertCalls = 0; g_InsertWhere = 0; g_InsertCount = 0; g_InsertVal = 0;
}

int main()
{
    int failures = 0;

    Elem storage[4] = { {10}, {20}, {30}, {40} };
    Elem valueSentinel; valueSentinel.value = 0x5151;

    CArrayElem c;
    c._First = &storage[0];
    c._Last  = &storage[4];            // size() = 4

    // _Newsize < size() : ERASE(begin()+_Newsize, end())
    Reset();
    c.resize(2, valueSentinel);
    if (g_EraseCalls != 1 || g_InsertCalls != 0 ||
        g_EraseF != (c._First + 2) || g_EraseL != c._Last)
    {
        std::printf("erase-path failed f=%p l=%p er=%d in=%d\n",
                    (void*)g_EraseF, (void*)g_EraseL, g_EraseCalls, g_InsertCalls);
        ++failures;
    }

    // _Newsize == size() : INSERT(end(), 0, val)
    Reset();
    c.resize(4, valueSentinel);
    if (g_InsertCalls != 1 || g_EraseCalls != 0 ||
        g_InsertWhere != c._Last || g_InsertCount != 0 ||
        g_InsertVal != &valueSentinel)
    {
        std::printf("insert-eq failed w=%p n=%u v=%p er=%d in=%d\n",
                    (void*)g_InsertWhere, g_InsertCount, (const void*)g_InsertVal,
                    g_EraseCalls, g_InsertCalls);
        ++failures;
    }

    // _Newsize > size() : INSERT(end(), _Newsize-size(), val)
    Reset();
    c.resize(7, valueSentinel);
    if (g_InsertCalls != 1 || g_EraseCalls != 0 ||
        g_InsertWhere != c._Last || g_InsertCount != 3 ||
        g_InsertVal != &valueSentinel)
    {
        std::printf("insert-gt failed w=%p n=%u v=%p er=%d in=%d\n",
                    (void*)g_InsertWhere, g_InsertCount, (const void*)g_InsertVal,
                    g_EraseCalls, g_InsertCalls);
        ++failures;
    }

    if (failures != 0)
    {
        std::printf("CARRAY_PUSH_BACK_004721B3_TEST FAIL count=%d\n", failures);
        return 1;
    }

    std::printf("CARRAY_PUSH_BACK_004721B3_TEST PASS\n");
    return 0;
}