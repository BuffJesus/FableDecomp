#include <cstdio>

struct CActiveFile {
    void* m_buffer;   // +0x00
    void Finalise();
    void OnReadFinished();
};

static int g_finalise_calls = 0;
static void* g_freed = (void*)0;

void CActiveFile::Finalise()
{
    g_finalise_calls++;
}

extern "C" void FreeBuffer(void* p)
{
    g_freed = p;
}

void CActiveFile::OnReadFinished()
{
    this->Finalise();
    if (this->m_buffer)
        FreeBuffer(this->m_buffer);
}

int main()
{
    // Case 1: non-null buffer -> Finalise called, buffer freed
    int dummy = 123;
    CActiveFile a;
    a.m_buffer = &dummy;
    a.OnReadFinished();
    bool ok1 = (g_finalise_calls == 1) && (g_freed == &dummy);

    // Case 2: null buffer -> Finalise called, no free
    g_finalise_calls = 0;
    g_freed = (void*)0;
    CActiveFile b;
    b.m_buffer = (void*)0;
    b.OnReadFinished();
    bool ok2 = (g_finalise_calls == 1) && (g_freed == (void*)0);

    if (ok1 && ok2) {
        printf("OnReadFinished_OK\n");
        return 0;
    }
    printf("FAIL c1=%d c2=%d\n", (int)ok1, (int)ok2);
    return 1;
}