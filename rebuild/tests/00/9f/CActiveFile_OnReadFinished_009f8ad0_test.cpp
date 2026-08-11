#include <cstdio>
#include <cstdlib>

extern "C" void __cdecl FreeIt(void* p);

struct CActiveFile
{
    void* m_ptr;
    void Helper();
    void OnReadFinished();
};

static int g_helperCalls = 0;
static int g_freeCalls = 0;
static void* g_freed = 0;

void CActiveFile::Helper()
{
    ++g_helperCalls;
}

extern "C" void __cdecl FreeIt(void* p)
{
    ++g_freeCalls;
    g_freed = p;
}

void CActiveFile::OnReadFinished()
{
    Helper();
    void* p = m_ptr;
    if (p)
        FreeIt(p);
}

int main()
{
    bool ok = true;

    // Case 1: m_ptr non-null -> Helper called once, FreeIt called with that ptr
    {
        g_helperCalls = g_freeCalls = 0; g_freed = 0;
        int sentinel = 42;
        CActiveFile a;
        a.m_ptr = &sentinel;
        a.OnReadFinished();
        if (g_helperCalls != 1) ok = false;
        if (g_freeCalls != 1) ok = false;
        if (g_freed != &sentinel) ok = false;
    }

    // Case 2: m_ptr null -> Helper called, FreeIt NOT called
    {
        g_helperCalls = g_freeCalls = 0; g_freed = 0;
        CActiveFile a;
        a.m_ptr = 0;
        a.OnReadFinished();
        if (g_helperCalls != 1) ok = false;
        if (g_freeCalls != 0) ok = false;
    }

    if (ok) { printf("ONREADFINISHED_OK\n"); return 0; }
    printf("ONREADFINISHED_FAIL\n"); return 1;
}