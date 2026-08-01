#include <cstdio>

// 0x50-byte POD element.
struct Elem {
    unsigned long m[20];   // sizeof == 0x50
};

struct CArrayElem {
    Elem* _First;  // +0x00
    Elem* _Last;   // +0x04

    Elem* begin() const { return _First; }
    Elem* end() const   { return _Last; }
    unsigned int size() const { return (unsigned int)(_Last - _First); }

    void erase(Elem* _F, Elem* _L);
    void _Insert_n(Elem* _Where, unsigned int _Count, const Elem& _Val);

    void resize(unsigned int _Newsize, const Elem& _Val);
};

void CArrayElem::resize(unsigned int _Newsize, const Elem& _Val)
{
    if (_Newsize < size())
        erase(begin() + _Newsize, end());
    else
        _Insert_n(end(), _Newsize - size(), _Val);
}

static int    g_EraseCalls;
static Elem*  g_EraseF;
static Elem*  g_EraseL;
static int    g_InsertCalls;
static Elem*  g_InsertWhere;
static unsigned int g_InsertCount;
static const Elem*  g_InsertVal;

void CArrayElem::erase(Elem* _F, Elem* _L)
{
    ++g_EraseCalls; g_EraseF = _F; g_EraseL = _L;
}
void CArrayElem::_Insert_n(Elem* _Where, unsigned int _Count, const Elem& _Val)
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
    static Elem storage[8];
    Elem val;

    CArrayElem a;
    a._First = &storage[0];
    a._Last  = &storage[4];   // size() == 4

    // _Newsize < size() -> ERASE(begin()+_Newsize, end())
    Reset();
    a.resize(2, val);
    if (g_EraseCalls != 1 || g_InsertCalls != 0 ||
        g_EraseF != a._First + 2 || g_EraseL != a._Last) {
        std::printf("erase-path failed F=%p L=%p e=%d i=%d\n",
                    (void*)g_EraseF, (void*)g_EraseL, g_EraseCalls, g_InsertCalls);
        ++failures;
    }

    // _Newsize == size() -> INSERT(end(), 0, val)
    Reset();
    a.resize(4, val);
    if (g_InsertCalls != 1 || g_EraseCalls != 0 ||
        g_InsertWhere != a._Last || g_InsertCount != 0 || g_InsertVal != &val) {
        std::printf("insert-eq failed W=%p c=%u v=%p e=%d i=%d\n",
                    (void*)g_InsertWhere, g_InsertCount, (const void*)g_InsertVal,
                    g_EraseCalls, g_InsertCalls);
        ++failures;
    }

    // _Newsize > size() -> INSERT(end(), _Newsize-size(), val)
    Reset();
    a.resize(7, val);
    if (g_InsertCalls != 1 || g_EraseCalls != 0 ||
        g_InsertWhere != a._Last || g_InsertCount != 3 || g_InsertVal != &val) {
        std::printf("insert-gt failed W=%p c=%u v=%p e=%d i=%d\n",
                    (void*)g_InsertWhere, g_InsertCount, (const void*)g_InsertVal,
                    g_EraseCalls, g_InsertCalls);
        ++failures;
    }

    if (failures != 0) {
        std::printf("CARRAY_PUSH_BACK_0047A407_TEST FAIL count=%d\n", failures);
        return 1;
    }
    std::printf("CARRAY_PUSH_BACK_0047A407_TEST PASS\n");
    return 0;
}