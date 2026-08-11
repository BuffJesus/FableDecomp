#include <cstdio>

static int g_helperCalled = 0;
static void* g_freed = 0;

extern "C" void __cdecl free_helper(void* p) { g_freed = p; }

struct CActiveFile
{
    void* m_ptr;
    void Helper();
    void OnReadFinished();
};

void CActiveFile::Helper() { g_helperCalled++; }

void CActiveFile::OnReadFinished()
{
    Helper();
    void* p = m_ptr;
    if (p)
        free_helper(p);
}

int main()
{
    int dummy = 0;
    CActiveFile a;
    a.m_ptr = &dummy;
    a.OnReadFinished();
    bool ok1 = (g_helperCalled == 1) && (g_freed == &dummy);

    g_helperCalled = 0; g_freed = 0;
    CActiveFile b;
    b.m_ptr = 0;
    b.OnReadFinished();
    bool ok2 = (g_helperCalled == 1) && (g_freed == 0);

    if (ok1 && ok2)
        printf("OnReadFinished_OK\n");
    else
        printf("FAIL h=%d\n", g_helperCalled);
    return (ok1 && ok2) ? 0 : 1;
}