#include <stdio.h>
#include <stdlib.h>

struct CActiveFile {
    void* buffer;
    int helper_called;
};

void __fastcall CActiveFile_Helper(CActiveFile* self)
{
    self->helper_called = 1;
}

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Helper(self);
    void* p = self->buffer;
    if (p)
        free(p);
}

int main()
{
    CActiveFile a;
    a.buffer = malloc(32);
    a.helper_called = 0;
    CActiveFile_OnReadFinished(&a);
    if (!a.helper_called) { printf("FAIL helper\n"); return 1; }

    CActiveFile b;
    b.buffer = 0;
    b.helper_called = 0;
    CActiveFile_OnReadFinished(&b);
    if (!b.helper_called) { printf("FAIL helper2\n"); return 1; }

    printf("OK_004e1bbb\n");
    return 0;
}