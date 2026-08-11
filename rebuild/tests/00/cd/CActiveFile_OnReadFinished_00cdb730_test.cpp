#include <cstdio>

static int g_helper = 0;
static void* g_freed = (void*)0;

void __fastcall CActiveFile_Helper(void* self) { g_helper++; (void)self; }
void CActiveFile_Free(void* p) { g_freed = p; }

struct CActiveFile
{
    void* m_buffer;
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    CActiveFile_Helper(this);
    void* p = this->m_buffer;
    if (p)
        CActiveFile_Free(p);
}

int main()
{
    CActiveFile a;
    int dummy = 7;
    a.m_buffer = &dummy;
    a.OnReadFinished();
    bool ok1 = (g_helper == 1) && (g_freed == &dummy);

    CActiveFile b;
    b.m_buffer = (void*)0;
    a.OnReadFinished();  // helper->2
    g_freed = (void*)0;
    b.OnReadFinished();
    bool ok2 = (g_helper == 3) && (g_freed == (void*)0);

    if (ok1 && ok2) { printf("ONREADFINISHED_OK\n"); return 0; }
    printf("FAIL h=%d\n", g_helper);
    return 1;
}