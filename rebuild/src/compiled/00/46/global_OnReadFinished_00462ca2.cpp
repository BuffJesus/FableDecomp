#include "rebuild_abi.h"

struct CActiveFile
{
    void* field_0;
};

extern "C" void __cdecl sub_79bd72(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr)
{
    void* p = thisptr->field_0;
    if (p != 0)
        sub_79bd72(p);
}