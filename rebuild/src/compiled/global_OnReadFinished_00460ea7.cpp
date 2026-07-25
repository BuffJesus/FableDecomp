#include "rebuild_abi.h"

// 0x00460ea7 CActiveFile::OnReadFinished (14 bytes)
// Loads this->m_ptr (this+0); if non-null, passes it to an engine
// callee (operator delete / free-style, cdecl with caller pop).
// mov eax,[ecx]; test eax,eax; je end; push eax; call rel32; pop ecx; ret

struct CActiveFile {
    void* m_ptr; // +0x00
};

extern "C" void __cdecl EngineFree_460ea7(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr)
{
    void* p = thisptr->m_ptr;
    if (p != 0) {
        EngineFree_460ea7(p);
    }
}