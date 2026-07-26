#include "rebuild_abi.h"

/* 0x00471f52  CActiveFile::OnReadFinished (14 bytes)
 * mov eax,[ecx]; test eax,eax; je end; push eax; call <free>; pop ecx; ret
 * A void __fastcall member that reads the pointer at this+0 and, when non-null,
 * passes it to a cdecl free function (1 arg cleaned by pop ecx).
 */

struct CActiveFile
{
    void* ptr; /* +0x00 */
};

/* cdecl callee; single pointer arg (caller cleans stack via pop ecx). */
extern "C" void __cdecl sub_78cac2(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr)
{
    void* p = thisptr->ptr;
    if (p)
    {
        sub_78cac2(p);
    }
}