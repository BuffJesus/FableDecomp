#include <cstdio>

static int g_helper_calls = 0;
static int g_free_calls = 0;
static void* g_freed = 0;

struct CActiveFile {
    void* pBuffer;
    void Helper();
    void OnReadFinished();
};

void CActiveFile::Helper() { g_helper_calls++; }

void EngineFree(void* p) { g_free_calls++; g_freed = p; }

void CActiveFile::OnReadFinished()
{
    Helper();
    if (pBuffer)
        EngineFree(pBuffer);
}

int main()
{
    // Case 1: non-null buffer -> helper called, buffer freed
    int dummy = 42;
    CActiveFile a;
    a.pBuffer = &dummy;
    a.OnReadFinished();
    bool ok1 = (g_helper_calls == 1) && (g_free_calls == 1) && (g_freed == &dummy);

    // Case 2: null buffer -> helper called, no free
    CActiveFile b;
    b.pBuffer = 0;
    b.OnReadFinished();
    bool ok2 = (g_helper_calls == 2) && (g_free_calls == 1);

    if (ok1 && ok2)
        printf("ONREADFINISHED_OK\n");
    else
        printf("FAIL h=%d f=%d\n", g_helper_calls, g_free_calls);
    return (ok1 && ok2) ? 0 : 1;
}