#pragma optimize("s",on)
#include <stdlib.h>

struct CActiveFile {
    void* buffer;
};

void __fastcall CActiveFile_Cleanup(CActiveFile* self);

void __fastcall OnReadFinished_3(CActiveFile* self)
{
    CActiveFile_Cleanup(self);
    void* p = self->buffer;
    if (p)
        free(p);
}