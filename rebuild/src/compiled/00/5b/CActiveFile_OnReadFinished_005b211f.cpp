#pragma optimize("s",on)
#include <stdlib.h>

struct CActiveFile {
    void* buf;
};

extern "C" void __fastcall Helper_16(CActiveFile* self);

void __fastcall OnReadFinished_16(CActiveFile* self)
{
    Helper_16(self);
    void* p = self->buf;
    if (p)
        free(p);
}