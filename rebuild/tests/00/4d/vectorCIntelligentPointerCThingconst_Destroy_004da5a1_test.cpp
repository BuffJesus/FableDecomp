#include <stdio.h>

static int g_calls = 0;
static int g_flagSum = 0;

struct CIntelligentPointer
{
    virtual void Release(int flag); // slot 0
    void* pThing;                   // +4
};

void CIntelligentPointer::Release(int) {}

void __fastcall _Destroy(CIntelligentPointer* first, CIntelligentPointer* last, void* alloc);

struct Impl : public CIntelligentPointer
{
    virtual void Release(int flag)
    {
        g_calls++;
        g_flagSum += flag;
        pThing = 0;
    }
};

int main()
{
    Impl arr[4];
    for (int i = 0; i < 4; ++i) arr[i].pThing = (void*)(long)(i + 1);

    CIntelligentPointer* base = arr;
    _Destroy(base, base + 4, (void*)0);

    // empty range: no calls
    _Destroy(base, base, (void*)0);

    if (g_calls == 4 && g_flagSum == 0)
    {
        printf("DESTROY_PASS calls=%d\n", g_calls);
        return 0;
    }
    printf("DESTROY_FAIL calls=%d flagSum=%d\n", g_calls, g_flagSum);
    return 1;
}