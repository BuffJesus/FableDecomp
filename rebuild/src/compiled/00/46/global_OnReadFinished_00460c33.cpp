#include "rebuild_abi.h"

// 0x00460c33 CActiveFile::OnReadFinished
// mov eax,[ecx]; test eax,eax; je end; push eax; call <free/delete>; pop ecx; ret

struct CActiveFile
{
    void* m_ptr; // +0x00
};

extern "C" void __cdecl engine_free_460c33(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr)
{
    void* p = thisptr->m_ptr;
    if (p != 0)
    {
        engine_free_460c33(p);
    }
}