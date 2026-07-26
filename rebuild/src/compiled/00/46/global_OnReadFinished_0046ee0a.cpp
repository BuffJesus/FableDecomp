#pragma optimize("s",on)
#include "rebuild_abi.h"

// Reconstruction of CActiveFile::OnReadFinished @ 0x0046ee0a (retail, 35 bytes).
// Retail is __fastcall(void); modeled as a free __fastcall(thisptr) accessor,
// byte-identical for a this-only member (ecx = thisptr).
//
// Disasm mapping:
//   push ecx            -> reserve a 1-byte local (in the pushed dword)
//   mov  esi,ecx        -> this
//   mov  edx,[esi+0x4]  -> edx = field_4
//   mov  ecx,[esi]      -> ecx = field_0 (this-of-call1)
//   lea  eax,[esp+0x7]  -> &local
//   push eax; call ...  -> __fastcall call1(field_0, field_4, &local)
//   mov  esi,[esi]      -> reload field_0
//   test esi,esi; je    -> if (field_0)
//   push esi; call ...; pop ecx -> __cdecl call2(field_0)

struct CActiveFile
{
    void* field_0;   // +0x0 : sub-object pointer (ecx of call1; tested; freed by call2)
    int   field_4;   // +0x4 : passed in edx to call1
};

// call1: ecx = field_0, edx = field_4, one pushed stack arg (&local byte).
extern void FABLE_FASTCALL CActiveFile_call1(void* thisp, int edx, char* p);

// call2: __cdecl free function taking field_0 as a stack arg (pop ecx cleanup).
extern void FABLE_CDECL CActiveFile_call2(void* p);

void FABLE_FASTCALL CActiveFile_OnReadFinished(CActiveFile* thisptr)
{
    char local;
    CActiveFile_call1(thisptr->field_0, thisptr->field_4, &local);
    if (thisptr->field_0 != 0)
        CActiveFile_call2(thisptr->field_0);
}