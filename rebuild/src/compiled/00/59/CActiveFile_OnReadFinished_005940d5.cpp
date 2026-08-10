#pragma optimize("s",on)
#include <stdlib.h>
struct CActiveFile { void* p0; };
extern "C" void __fastcall Base_Read(CActiveFile* self);
void __fastcall OnReadFinished_15(CActiveFile* self)
{
    Base_Read(self);
    void* p = self->p0;
    if (p)
        free(p);
}