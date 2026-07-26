#include "rebuild_abi.h"

struct CActiveFile
{
    void* m_ptr;   // +0x00
};

/* single-arg __cdecl callee (relocation-masked target; likely operator delete / free) */
extern "C" void __cdecl sub_78C9DC_free(void* p);

/* __fastcall accessor modeled as free __fastcall(this) — byte-identical for the lander */
void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr)
{
    void* p = thisptr->m_ptr;      /* mov eax,[ecx] ; test eax,eax */
    if (p != 0)                    /* je end */
        sub_78C9DC_free(p);        /* push eax ; call ; pop ecx (cdecl 1 arg) */
}