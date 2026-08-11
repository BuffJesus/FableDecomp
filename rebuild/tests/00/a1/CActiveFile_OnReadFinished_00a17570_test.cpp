#include <cstdio>

static int g_notify = 0;
static void* g_released = (void*)-1;

extern "C" void __cdecl ReleaseBuf(void* p) { g_released = p; }

struct CActiveFile {
    void* m_ptr;
    void Notify();
    void OnReadFinished();
};

void CActiveFile::Notify() { g_notify++; }

void CActiveFile::OnReadFinished()
{
    Notify();
    void* p = m_ptr;
    if (p)
        ReleaseBuf(p);
}

int main()
{
    // case 1: non-null ptr -> notified + released with that ptr
    CActiveFile a;
    int dummy = 7;
    a.m_ptr = &dummy;
    g_notify = 0; g_released = (void*)-1;
    a.OnReadFinished();
    bool ok1 = (g_notify == 1) && (g_released == &dummy);

    // case 2: null ptr -> notified but NOT released
    CActiveFile b;
    b.m_ptr = 0;
    g_notify = 0; g_released = (void*)-1;
    b.OnReadFinished();
    bool ok2 = (g_notify == 1) && (g_released == (void*)-1);

    if (ok1 && ok2) {
        printf("ONREADFINISHED_OK\n");
        return 0;
    }
    printf("FAIL n=%d\n", g_notify);
    return 1;
}