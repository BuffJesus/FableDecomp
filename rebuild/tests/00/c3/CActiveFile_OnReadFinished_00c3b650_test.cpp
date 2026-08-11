#include <cstdio>

static int g_freed = 0;
static void* g_last = 0;

void operator_delete_thunk(void* p) { g_freed++; g_last = p; }

struct CActiveFile {
    void* m_buffer;
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    if (this->m_buffer != 0) {
        operator_delete_thunk(this->m_buffer);
        this->m_buffer = 0;
    }
}

int main()
{
    int dummy = 0;
    CActiveFile a;
    a.m_buffer = &dummy;
    a.OnReadFinished();
    if (g_freed != 1) { printf("FAIL freed=%d\n", g_freed); return 1; }
    if (g_last != &dummy) { printf("FAIL ptr\n"); return 1; }
    if (a.m_buffer != 0) { printf("FAIL notnull\n"); return 1; }

    // null path: no free
    CActiveFile b;
    b.m_buffer = 0;
    b.OnReadFinished();
    if (g_freed != 1) { printf("FAIL freed2=%d\n", g_freed); return 1; }

    printf("ONREADFINISHED_OK\n");
    return 0;
}