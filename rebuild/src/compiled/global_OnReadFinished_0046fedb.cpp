#include "rebuild_abi.h"

struct CActiveFile {
    void* field0;
};

extern "C" void __cdecl sub_free_46fedb(void* p);

void __fastcall OnReadFinished_0046fedb(CActiveFile* thisptr)
{
    void* p = thisptr->field0;
    if (p) {
        sub_free_46fedb(p);
    }
}