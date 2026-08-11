#include <stdio.h>

struct CActiveFile {
    void* p;
};

static int g_base_called = 0;
static void* g_freed = (void*)-1;

void __fastcall CActiveFile_BaseReadDone(void* self)
{
    (void)self;
    g_base_called++;
}

void FreePtr(void* p)
{
    g_freed = p;
}

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self);

int main()
{
    // Case 1: p non-null -> base called, FreePtr(p) called with p
    int dummy = 0;
    CActiveFile a;
    a.p = &dummy;
    g_base_called = 0;
    g_freed = (void*)-1;
    CActiveFile_OnReadFinished(&a);
    bool ok1 = (g_base_called == 1) && (g_freed == (void*)&dummy);

    // Case 2: p null -> base called, FreePtr NOT called
    CActiveFile b;
    b.p = 0;
    g_base_called = 0;
    g_freed = (void*)-1;
    CActiveFile_OnReadFinished(&b);
    bool ok2 = (g_base_called == 1) && (g_freed == (void*)-1);

    if (ok1 && ok2)
        printf("004c5c60_TEST PASS\n");
    else
        printf("FAIL ok1=%d ok2=%d\n", (int)ok1, (int)ok2);
    return 0;
}