#include <cstdio>

static int g_pre = 0;
static void *g_freed = (void*)-1;

extern "C" void __cdecl free_buf(void *p) { g_freed = p; }

struct CActiveFile
{
    void *m_buffer;
    void PreFinish() { g_pre++; }
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    PreFinish();
    void *p = m_buffer;
    if (p)
        free_buf(p);
}

int main()
{
    // case 1: non-null buffer -> pre called, freed with buffer
    int dummy;
    CActiveFile a; a.m_buffer = &dummy;
    g_pre = 0; g_freed = (void*)-1;
    a.OnReadFinished();
    bool ok1 = (g_pre == 1) && (g_freed == (void*)&dummy);

    // case 2: null buffer -> pre called, free NOT called
    CActiveFile b; b.m_buffer = 0;
    g_pre = 0; g_freed = (void*)-1;
    b.OnReadFinished();
    bool ok2 = (g_pre == 1) && (g_freed == (void*)-1);

    if (ok1 && ok2) { printf("ONREADFINISHED_OK\n"); return 0; }
    printf("FAIL %d %d\n", (int)ok1, (int)ok2);
    return 1;
}