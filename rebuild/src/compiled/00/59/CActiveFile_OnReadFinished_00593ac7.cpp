#pragma optimize("s",on)
#include <stdlib.h>

struct CActiveFile {
    void* m0;
};

void __fastcall helper13(CActiveFile* self);

void __fastcall OnReadFinished_13(CActiveFile* self)
{
    helper13(self);
    void* p = self->m0;
    if (p)
        free(p);
}