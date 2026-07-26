#include "rebuild_abi.h"

struct CActiveFile {
    void* m_ptr; // +0x00
};

extern "C" void __cdecl free_461eaf(void* p);

void __fastcall OnReadFinished_461eaf(CActiveFile* thisptr)
{
    void* p = thisptr->m_ptr;
    if (p) {
        free_461eaf(p);
    }
}