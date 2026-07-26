#include "rebuild_abi.h"

// 0x0045bce4 CActiveFile::OnReadFinished
// Loads this->m_ptr (offset 0); if non-null, passes it to a cdecl free
// function (result discarded), then returns.

struct CActiveFile
{
    void* m_ptr; // +0x00
};

extern "C" void sub_7a2d30(void* p); // cdecl callee; address relocation-masked

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr)
{
    void* p = thisptr->m_ptr;
    if (p != 0)
    {
        sub_7a2d30(p);
    }
}