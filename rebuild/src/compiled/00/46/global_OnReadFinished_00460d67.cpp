#include "rebuild_abi.h"

struct CActiveFile
{
    void* m_ptr; /* +0x00 */
};

extern "C" void __cdecl CActiveFile_ReleaseBuffer(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr)
{
    void* p = thisptr->m_ptr;
    if (p != 0)
    {
        CActiveFile_ReleaseBuffer(p);
    }
}