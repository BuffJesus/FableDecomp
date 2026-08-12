#include <cstdio>

static int g_called = 0;
static void* g_arg = 0;

void __cdecl CActiveFile_helper(void* p) { g_called++; g_arg = p; }

struct CActiveFile {
    void* m_pReader;
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    if (m_pReader != 0)
        CActiveFile_helper(m_pReader);
}

int main()
{
    int ok = 1;

    // null reader -> helper not called
    CActiveFile a;
    a.m_pReader = 0;
    g_called = 0; g_arg = (void*)-1;
    a.OnReadFinished();
    if (g_called != 0) ok = 0;

    // non-null reader -> helper called with that pointer
    int dummy = 7;
    CActiveFile b;
    b.m_pReader = &dummy;
    g_called = 0; g_arg = 0;
    b.OnReadFinished();
    if (g_called != 1) ok = 0;
    if (g_arg != &dummy) ok = 0;

    if (ok) printf("ONREADFINISHED_OK\n");
    else    printf("FAIL\n");
    return ok ? 0 : 1;
}