#pragma optimize("s",on)
#include <stdlib.h>

struct CActiveFile {
    void* buf;
};

extern "C" void __fastcall Helper_OnReadFinished(CActiveFile* self);

void __fastcall OnReadFinished(CActiveFile* self)
{
    Helper_OnReadFinished(self);
    void* p = self->buf;
    if (p)
        free(p);
}