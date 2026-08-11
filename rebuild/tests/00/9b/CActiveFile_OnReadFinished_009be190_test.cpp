#include <cstdio>

static int g_freed = 0;
static int g_helper = 0;
static void* g_lastfree = 0;

void some_free(void* p) { g_freed++; g_lastfree = p; }

struct CActiveFile {
    void* buffer;
    void Helper();
    void OnReadFinished();
};

void CActiveFile::Helper() { g_helper++; }

void CActiveFile::OnReadFinished()
{
    this->Helper();
    void* p = this->buffer;
    if (p)
        some_free(p);
}

int main()
{
    int dummy = 42;
    CActiveFile a;
    a.buffer = &dummy;
    a.OnReadFinished();
    if (g_helper != 1 || g_freed != 1 || g_lastfree != &dummy) { printf("FAIL\n"); return 1; }

    CActiveFile b;
    b.buffer = 0;
    b.OnReadFinished();
    if (g_helper != 2 || g_freed != 1) { printf("FAIL\n"); return 1; }

    printf("OARF_OK\n");
    return 0;
}