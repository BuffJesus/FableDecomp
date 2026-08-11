#include <stdio.h>
#include <stdlib.h>

extern "C" void __cdecl free_helper(void* p);

struct CActiveFile {
    void* m_buffer;   // +0x00
    void Helper();
    void OnReadFinished();
};

static int g_helper_called = 0;
static void* g_freed = (void*)-1;

void CActiveFile::Helper() { g_helper_called++; }
extern "C" void __cdecl free_helper(void* p) { g_freed = p; free(p); }

void CActiveFile::OnReadFinished()
{
    this->Helper();
    void* p = this->m_buffer;
    if (p)
        free_helper(p);
}

int main()
{
    // Case 1: non-null buffer -> helper called, buffer freed
    CActiveFile a;
    void* buf = malloc(16);
    a.m_buffer = buf;
    g_helper_called = 0;
    g_freed = (void*)-1;
    a.OnReadFinished();
    bool ok1 = (g_helper_called == 1) && (g_freed == buf);

    // Case 2: null buffer -> helper called, nothing freed
    CActiveFile b;
    b.m_buffer = 0;
    g_helper_called = 0;
    g_freed = (void*)-1;
    b.OnReadFinished();
    bool ok2 = (g_helper_called == 1) && (g_freed == (void*)-1);

    if (ok1 && ok2)
        printf("OnReadFinished_OK\n");
    else
        printf("OnReadFinished_FAIL h1=%d h2 case2\n", g_helper_called);
    return (ok1 && ok2) ? 0 : 1;
}