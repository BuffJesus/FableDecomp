#pragma optimize("s",on)
#include "rebuild_abi.h"

struct CActiveFile_0046638b {
    void* field0;   // +0x00
    void* field4;   // +0x04
};

// First callee: fastcall taking ecx=[this+0], edx=[this+4], plus one stack arg (out byte ptr).
extern void __fastcall sub_fastcall_0046638b(void* a, void* b, char* out);
// Second callee: cdecl free function taking one pointer arg (caller cleans via pop ecx).
extern void __cdecl sub_cdecl_0046638b(void* p);

// 0x0046638b  CActiveFile::OnReadFinished  (__fastcall, void, 1 param)
void __fastcall OnReadFinished_0046638b(CActiveFile_0046638b* thisptr)
{
    char local;
    sub_fastcall_0046638b(thisptr->field0, thisptr->field4, &local);
    void* p = thisptr->field0;
    if (p) {
        sub_cdecl_0046638b(p);
    }
}