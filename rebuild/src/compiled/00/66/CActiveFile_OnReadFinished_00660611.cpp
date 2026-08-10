#pragma optimize("s",on)
#include <stdlib.h>

struct CActiveFile {
    void* buf;
};

extern "C" void __fastcall Helper_660611(CActiveFile* self);

void __fastcall OnReadFinished_660611(CActiveFile* self)
{
    Helper_660611(self);
    void* p = self->buf;
    if (p)
        free(p);
}