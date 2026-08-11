#include <stdio.h>

static int g_processReadCalls = 0;
static int g_releaseCalls = 0;
static void* g_releasedPtr = 0;

struct CActiveFile
{
    void* field0;
    void ProcessRead();
    void OnReadFinished();
};

void CActiveFile::ProcessRead()
{
    g_processReadCalls++;
}

extern "C" void __cdecl ReleaseBuffer(void* p)
{
    g_releaseCalls++;
    g_releasedPtr = p;
}

void CActiveFile::OnReadFinished()
{
    this->ProcessRead();
    void* p = this->field0;
    if (p)
        ReleaseBuffer(p);
}

int main()
{
    int dummy = 0;
    // case 1: field0 non-null -> release called with that ptr
    CActiveFile a;
    a.field0 = &dummy;
    a.OnReadFinished();
    bool ok1 = (g_processReadCalls == 1) && (g_releaseCalls == 1) && (g_releasedPtr == &dummy);

    // case 2: field0 null -> release NOT called, ProcessRead still called
    CActiveFile b;
    b.field0 = 0;
    b.OnReadFinished();
    bool ok2 = (g_processReadCalls == 2) && (g_releaseCalls == 1);

    if (ok1 && ok2)
        printf("ONREADFINISHED_OK\n");
    else
        printf("FAIL p=%d r=%d\n", g_processReadCalls, g_releaseCalls);
    return (ok1 && ok2) ? 0 : 1;
}