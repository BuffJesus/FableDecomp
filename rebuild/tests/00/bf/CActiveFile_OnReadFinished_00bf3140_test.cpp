#include <stdio.h>

struct CActiveFile {
    void* pReader;
};

static int g_called = 0;
static void* g_lastArg = 0;

void __stdcall CActiveFile_sub_bf29e0(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self);

// provide the extern
void __stdcall CActiveFile_sub_bf29e0(void* p) {
    g_called++;
    g_lastArg = p;
}

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    void* p = self->pReader;
    if (p) {
        CActiveFile_sub_bf29e0(p);
        self->pReader = 0;
    }
}

int main()
{
    // Case 1: null pointer -> nothing happens
    CActiveFile a; a.pReader = 0;
    CActiveFile_OnReadFinished(&a);
    if (g_called != 0) { printf("FAIL null\n"); return 1; }

    // Case 2: non-null -> callee called with the pointer, field nulled
    int dummy;
    CActiveFile b; b.pReader = &dummy;
    CActiveFile_OnReadFinished(&b);
    if (g_called != 1) { printf("FAIL called\n"); return 1; }
    if (g_lastArg != &dummy) { printf("FAIL arg\n"); return 1; }
    if (b.pReader != 0) { printf("FAIL nulled\n"); return 1; }

    printf("00bf3140_TEST PASS\n");
    return 0;
}