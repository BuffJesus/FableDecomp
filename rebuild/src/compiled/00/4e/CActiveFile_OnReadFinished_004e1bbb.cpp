#pragma optimize("s",on)
#include <stdlib.h>

struct CActiveFile {
    void* buffer;
};

extern void __fastcall CActiveFile_Helper(CActiveFile* self);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Helper(self);
    void* p = self->buffer;
    if (p)
        free(p);
}