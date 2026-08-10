#pragma optimize("s",on)
#include <stdlib.h>

struct CActiveFile;
extern "C" void __fastcall CActiveFile_helper(CActiveFile* self);

struct CActiveFile {
    void* ptr;
};

extern "C" void __fastcall OnReadFinished_2(CActiveFile* self)
{
    CActiveFile_helper(self);
    void* p = *(void**)self;
    if (p)
        free(p);
}