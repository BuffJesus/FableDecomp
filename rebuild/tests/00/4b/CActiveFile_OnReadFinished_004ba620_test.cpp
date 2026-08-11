#include <stdio.h>

struct CActiveFile;
extern void __fastcall BaseOnReadFinished(CActiveFile *self);

struct CActiveFile {
    void *m_buffer;
    void __fastcall OnReadFinished();
};

static int g_baseCalled = 0;
static int g_freed = 0;
static void *g_freedPtr = 0;

void __fastcall BaseOnReadFinished(CActiveFile *) { g_baseCalled++; }

static void myfree(void *p) { g_freed++; g_freedPtr = p; }

void __fastcall CActiveFile::OnReadFinished() {
    BaseOnReadFinished(this);
    void *p = this->m_buffer;
    if (p) {
        myfree(p);
    }
}

int main() {
    int dummy = 0;
    CActiveFile a;
    a.m_buffer = &dummy;
    g_baseCalled = g_freed = 0; g_freedPtr = 0;
    a.OnReadFinished();
    bool ok1 = (g_baseCalled == 1) && (g_freed == 1) && (g_freedPtr == &dummy);

    CActiveFile b;
    b.m_buffer = 0;
    g_baseCalled = g_freed = 0; g_freedPtr = 0;
    b.OnReadFinished();
    bool ok2 = (g_baseCalled == 1) && (g_freed == 0);

    if (ok1 && ok2) printf("004ba620_TEST PASS\n");
    else printf("FAIL b=%d f=%d\n", g_baseCalled, g_freed);
    return 0;
}