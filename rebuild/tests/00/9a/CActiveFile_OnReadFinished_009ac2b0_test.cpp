#include <stdio.h>

extern "C" void freefn(void* p);

struct CActiveFile {
    void* buffer;
    void Helper();
    void OnReadFinished();
};

static int g_helper_calls = 0;
static void* g_freed = (void*)-1;

void CActiveFile::Helper() { g_helper_calls++; }
extern "C" void freefn(void* p) { g_freed = p; }

void CActiveFile::OnReadFinished()
{
    this->Helper();
    void* p = this->buffer;
    if (p != 0)
        freefn(p);
}

int main()
{
    // Case 1: buffer non-null -> helper called, freefn called with buffer
    int dummy = 42;
    CActiveFile a;
    a.buffer = &dummy;
    g_helper_calls = 0; g_freed = (void*)-1;
    a.OnReadFinished();
    bool ok1 = (g_helper_calls == 1) && (g_freed == &dummy);

    // Case 2: buffer null -> helper called, freefn NOT called
    CActiveFile b;
    b.buffer = 0;
    g_helper_calls = 0; g_freed = (void*)-1;
    b.OnReadFinished();
    bool ok2 = (g_helper_calls == 1) && (g_freed == (void*)-1);

    if (ok1 && ok2) { printf("ORF_PASS\n"); return 0; }
    printf("ORF_FAIL h=%d\n", g_helper_calls);
    return 1;
}