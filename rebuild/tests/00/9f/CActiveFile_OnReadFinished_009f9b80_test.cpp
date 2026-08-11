#include <cstdio>
#include <cstdlib>

static int g_helper_called = 0;
static int g_freed = 0;
static void* g_freed_ptr = 0;

extern "C" void __cdecl free_impl(void* p) { g_freed = 1; g_freed_ptr = p; }

struct CActiveFile {
    void* buffer;
    void Helper();
    void OnReadFinished();
};

void CActiveFile::Helper() { g_helper_called = 1; }

void CActiveFile::OnReadFinished()
{
    this->Helper();
    void* p = this->buffer;
    if (p)
        free_impl(p);
}

int main()
{
    int slot = 0;
    void* pslot = &slot;
    CActiveFile a;
    a.buffer = pslot;
    a.OnReadFinished();
    bool ok1 = (g_helper_called == 1) && (g_freed == 1) && (g_freed_ptr == pslot);

    g_helper_called = 0; g_freed = 0; g_freed_ptr = 0;
    CActiveFile b;
    b.buffer = 0;
    b.OnReadFinished();
    bool ok2 = (g_helper_called == 1) && (g_freed == 0);

    if (ok1 && ok2)
        printf("ONREADFINISHED_OK\n");
    else
        printf("FAIL h=%d f=%d\n", g_helper_called, g_freed);
    return (ok1 && ok2) ? 0 : 1;
}