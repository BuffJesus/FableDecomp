#include <stdio.h>
#include <stdlib.h>

struct CActiveFile { void* buffer; };
static int g_cleanup=0;
static int g_freed=0;
void __fastcall CActiveFile_Cleanup(CActiveFile* self){ (void)self; g_cleanup++; }

void __fastcall OnReadFinished_3(CActiveFile* self)
{
    CActiveFile_Cleanup(self);
    void* p = self->buffer;
    if (p) free(p);
}

int main(){
    CActiveFile a;
    a.buffer = malloc(16);
    OnReadFinished_3(&a);
    if(g_cleanup!=1){ printf("FAIL cleanup\n"); return 1; }

    CActiveFile b;
    b.buffer = 0;
    OnReadFinished_3(&b);
    if(g_cleanup!=2){ printf("FAIL cleanup2\n"); return 1; }

    printf("OK_005bb04f\n");
    return 0;
}