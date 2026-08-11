#include <cstdio>

static int g_calls = 0;
static int g_lastFlag = -999;

struct CIntelPtr {
    virtual void vfn0(int flag);
    char pad[0x48 - 4];
};

void __fastcall vector_Destroy(CIntelPtr* first, CIntelPtr* last, void* alloc);

// Provide the virtual out-of-line so it isn't inlined away in the test.
void CIntelPtr::vfn0(int flag) { g_calls++; g_lastFlag = flag; }

int main()
{
    const int N = 5;
    CIntelPtr arr[N];
    vector_Destroy(&arr[0], &arr[N], (void*)0);
    if (g_calls == N && g_lastFlag == 0)
        printf("DESTROY_OK\n");
    else
        printf("DESTROY_FAIL calls=%d flag=%d\n", g_calls, g_lastFlag);

    // empty range: no calls
    g_calls = 0;
    vector_Destroy(&arr[0], &arr[0], (void*)0);
    if (g_calls == 0)
        printf("EMPTY_OK\n");
    return (g_calls == 0) ? 0 : 1;
}