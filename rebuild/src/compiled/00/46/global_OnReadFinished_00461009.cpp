#include "rebuild_abi.h"

// 0x00461009  CActiveFile::OnReadFinished (14 bytes)
// Loads this+0x00, and if non-null, passes it to a global free-like callee.
// __fastcall accessor modeled as free __fastcall(objptr).

struct CActiveFile
{
    void* ptr; // +0x00
};

// relocation-masked engine callee (operator delete / free-like)
extern "C" void __cdecl fable_free_461009(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr)
{
    void* p = thisptr->ptr;   // mov eax,[ecx]
    if (p != 0)               // test eax,eax / je
    {
        fable_free_461009(p); // push eax / call ; pop ecx
    }
}