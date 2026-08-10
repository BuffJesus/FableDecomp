#include <stdio.h>
#include <stdlib.h>

struct CActiveFile {
    void* buf;
};

static int g_pre = 0;
extern "C" void __fastcall CActiveFile_PreRead(CActiveFile* self) { g_pre = 1; (void)self; }

void __fastcall OnReadFinished(CActiveFile* self);

int main()
{
    CActiveFile a;
    a.buf = malloc(16);
    OnReadFinished(&a);
    if (!g_pre) { printf("FAIL pre\n"); return 1; }

    CActiveFile b;
    b.buf = 0;
    g_pre = 0;
    OnReadFinished(&b);
    if (!g_pre) { printf("FAIL pre2\n"); return 1; }

    printf("OK_00660656\n");
    return 0;
}