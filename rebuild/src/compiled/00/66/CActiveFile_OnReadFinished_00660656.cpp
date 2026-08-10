#pragma optimize("s",on)
#include <stdlib.h>

struct CActiveFile {
    void* buf;
};

extern "C" void __fastcall CActiveFile_PreRead(CActiveFile* self);

void __fastcall OnReadFinished(CActiveFile* self)
{
    CActiveFile_PreRead(self);
    void* p = self->buf;
    if (p)
        free(p);
}