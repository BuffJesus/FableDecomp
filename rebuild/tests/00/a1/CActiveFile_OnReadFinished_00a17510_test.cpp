#include <cstdio>
#include <cstdlib>
#include <new>

struct CActiveFile {
    void* m_data;
    void Finish();
    void OnReadFinished();
};

static int g_finishCalled = 0;
static void* g_lastFreed = (void*)0;

// definition for the decl-only helper referenced by the source
void CActiveFile::Finish() { g_finishCalled++; }

// observe the free
void operator delete(void* p) throw() {
    g_lastFreed = p;
    if (p) std::free(p);
}

int main()
{
    // case 1: non-null m_data -> Finish runs, pointer freed
    CActiveFile a;
    void* blk = std::malloc(32);
    a.m_data = blk;
    g_finishCalled = 0; g_lastFreed = (void*)0;
    a.OnReadFinished();

    bool ok1 = (g_finishCalled == 1) && (g_lastFreed == blk);

    // case 2: null m_data -> Finish runs, no free
    CActiveFile b;
    b.m_data = (void*)0;
    g_finishCalled = 0; g_lastFreed = (void*)0;
    b.OnReadFinished();

    bool ok2 = (g_finishCalled == 1) && (g_lastFreed == (void*)0);

    if (ok1 && ok2) {
        std::printf("ONREADFINISHED_OK\n");
        return 0;
    }
    std::printf("FAIL c1=%d c2=%d\n", (int)ok1, (int)ok2);
    return 1;
}