#include <cstdio>
#include <cstdlib>

struct CActiveFile;

static int g_subCalled = 0;
static void* g_freed = 0;

void __fastcall CActiveFile_sub_834F90(CActiveFile* self);
void  __cdecl  free_helper(void* p);

struct CActiveFile
{
    void* m_ptr;
};

void __fastcall CActiveFile_sub_834F90(CActiveFile* /*self*/) { g_subCalled++; }
void  __cdecl  free_helper(void* p) { g_freed = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self);

int main()
{
    // Case 1: non-null pointer -> free_helper called with it
    CActiveFile a;
    int dummy = 0;
    a.m_ptr = &dummy;
    g_subCalled = 0; g_freed = 0;
    CActiveFile_OnReadFinished(&a);
    bool ok1 = (g_subCalled == 1) && (g_freed == &dummy);

    // Case 2: null pointer -> sub called, free NOT called
    CActiveFile b;
    b.m_ptr = 0;
    g_subCalled = 0; g_freed = (void*)0x1;
    CActiveFile_OnReadFinished(&b);
    bool ok2 = (g_subCalled == 1) && (g_freed == (void*)0x1);

    if (ok1 && ok2) {
        printf("ONREADFINISHED_OK\n");
        return 0;
    }
    printf("FAIL s=%d\n", g_subCalled);
    return 1;
}