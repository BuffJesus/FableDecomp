#include "rebuild_abi.h"

// 0x0046131b  CActiveFile::OnReadFinished (14 bytes)
// this+0x00 holds a pointer; if non-null, pass it to a cdecl free function.

extern "C" void __cdecl sub_79d6f9(void* p);

struct CActiveFile
{
    void* field_0;
};

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr)
{
    void* p = thisptr->field_0;
    if (p != 0)
    {
        sub_79d6f9(p);
    }
}