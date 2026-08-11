#include <cstdio>

static int g_helper = 0;
static void* g_freed = (void*)0;

struct CActiveFile {
    void* buffer;
    void Helper();
    void OnReadFinished();
};

void CActiveFile::Helper() { g_helper++; }
void freemem(void* p) { g_freed = p; }

void CActiveFile::OnReadFinished()
{
    Helper();
    void* p = buffer;
    if (p)
        freemem(p);
}

int main()
{
    int dummy = 0;
    CActiveFile a;
    a.buffer = &dummy;
    a.OnReadFinished();

    CActiveFile b;
    b.buffer = (void*)0;
    b.OnReadFinished();

    if (g_helper == 2 && g_freed == &dummy)
        printf("ONREADFIN_OK\n");
    else
        printf("FAIL h=%d\n", g_helper);
    return (g_helper == 2 && g_freed == &dummy) ? 0 : 1;
}