#pragma optimize("s",on)
#include <stdlib.h>

struct CActiveFile {
    void* buf;
};

extern "C" void __fastcall Helper_65f1fa(CActiveFile* self);

void __fastcall OnReadFinished_65f1fa(CActiveFile* self)
{
    Helper_65f1fa(self);
    void* p = self->buf;
    if (p)
        free(p);
}