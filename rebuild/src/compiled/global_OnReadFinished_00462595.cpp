#include "rebuild_abi.h"

struct CActiveFile
{
    void* m_ptr; /* +0x00 */
};

/* cdecl callee taking the pointer (relocation-masked target) */
extern void __cdecl CActiveFile_ReleaseCallee(void* p);

/* __fastcall accessor modeled as free __fastcall(thisptr) */
void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr)
{
    void* p = thisptr->m_ptr;   /* mov eax,[ecx] */
    if (p != 0)                 /* test eax,eax / je */
    {
        CActiveFile_ReleaseCallee(p); /* push eax / call / pop ecx */
    }
}