#include <cstdio>

struct Elem {
    virtual void dtor(int flag);
    char pad[0x30 - 4];
};

void __fastcall _Destroy(Elem* first, Elem* last, void* alloc)
{
    for (; first != last; ++first)
        first->dtor(0);
    (void)alloc;
}

static int g_calls = 0;
static int g_lastFlag = -1;

struct TestElem {
    void* vptr;
    char pad[0x30 - 4];
};

void __fastcall test_dtor(TestElem* self, int /*edx*/, int flag)
{
    (void)self;
    g_calls++;
    g_lastFlag = flag;
}

int main()
{
    void* vt[1];
    vt[0] = (void*)&test_dtor;

    const int N = 5;
    TestElem arr[N];
    for (int i = 0; i < N; ++i) arr[i].vptr = vt;

    Elem* first = reinterpret_cast<Elem*>(&arr[0]);
    Elem* last = reinterpret_cast<Elem*>(&arr[N]);
    _Destroy(first, last, (void*)0);
    bool fullOk = (g_calls == N && g_lastFlag == 0);

    // empty range
    g_calls = 0;
    _Destroy(first, first, (void*)0);
    bool emptyOk = (g_calls == 0);

    if (fullOk && emptyOk && sizeof(TestElem) == 0x30) {
        printf("DESTROY_RANGE_OK\n");
        return 0;
    }
    printf("FAIL calls=%d flag=%d empty=%d sz=%d\n", g_calls, g_lastFlag, (int)emptyOk, (int)sizeof(TestElem));
    return 1;
}