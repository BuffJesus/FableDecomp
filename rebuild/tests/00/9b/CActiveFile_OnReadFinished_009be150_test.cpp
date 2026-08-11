#include <cstdio>

static int g_helperCalls = 0;
static void* g_freed = (void*)-1;

struct CActiveFile {
    void* m_buffer;
    void Helper();
    void OnReadFinished();
};

void CActiveFile::Helper() { g_helperCalls++; }

extern "C" void __cdecl FreeBuffer(void* p) { g_freed = p; }

void CActiveFile::OnReadFinished()
{
    this->Helper();
    void* buf = this->m_buffer;
    if (buf)
        FreeBuffer(buf);
}

int main()
{
    // Case 1: non-null buffer -> helper called, buffer freed
    CActiveFile a;
    int dummy = 0;
    a.m_buffer = &dummy;
    g_helperCalls = 0; g_freed = (void*)-1;
    a.OnReadFinished();
    bool ok1 = (g_helperCalls == 1) && (g_freed == &dummy);

    // Case 2: null buffer -> helper called, no free
    CActiveFile b;
    b.m_buffer = 0;
    g_helperCalls = 0; g_freed = (void*)-1;
    b.OnReadFinished();
    bool ok2 = (g_helperCalls == 1) && (g_freed == (void*)-1);

    if (ok1 && ok2) { printf("ONREADFINISHED_OK\n"); return 0; }
    printf("FAIL h=%d\n", g_helperCalls);
    return 1;
}