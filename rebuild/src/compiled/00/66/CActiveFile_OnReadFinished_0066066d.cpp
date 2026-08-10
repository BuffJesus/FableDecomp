#pragma optimize("s",on)
#include <stdlib.h>

struct CActiveFile {
    void* buf;
};

extern "C" void __fastcall Helper_66066d(CActiveFile* self);

void __fastcall OnReadFinished_66066d(CActiveFile* self)
{
    Helper_66066d(self);
    void* p = self->buf;
    if (p)
        free(p);
}