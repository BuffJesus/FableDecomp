#include <stdio.h>
#include <stdlib.h>

// Independent behaviour reimplementation of CActiveFile::OnReadFinished.

struct CActiveFile
{
    void* field0;
    void Helper();
    void OnReadFinished();
};

static int g_helperCalls = 0;
static void* g_freed = (void*)0;

void CActiveFile::Helper() { ++g_helperCalls; }

extern "C" void __cdecl FreeMem(void* p) { g_freed = p; }

void CActiveFile::OnReadFinished()
{
    this->Helper();
    void* p = this->field0;
    if (p)
        FreeMem(p);
}

int main()
{
    int dummy = 0;
    // Case 1: field0 non-null -> Helper called AND FreeMem(field0)
    g_helperCalls = 0; g_freed = (void*)0;
    CActiveFile a;
    a.field0 = &dummy;
    a.OnReadFinished();
    bool ok1 = (g_helperCalls == 1) && (g_freed == &dummy);

    // Case 2: field0 null -> Helper called, FreeMem NOT called
    g_helperCalls = 0; g_freed = (void*)0;
    CActiveFile b;
    b.field0 = (void*)0;
    b.OnReadFinished();
    bool ok2 = (g_helperCalls == 1) && (g_freed == (void*)0);

    if (ok1 && ok2) {
        printf("ONREADFINISHED_OK\n");
        return 0;
    }
    printf("FAIL c1=%d c2=%d\n", (int)ok1, (int)ok2);
    return 1;
}