#pragma optimize("s",on)
#include "rebuild_abi.h"

// CActiveFile::OnReadFinished @ 0x00471df4
//
// thisptr (esi):
//   +0x00  Reader*    m_reader   (object with a __fastcall member taking edx + &byte)
//   +0x04  fable_i32  m_status   (loaded into edx -> __fastcall 2nd register arg)
//
// Retail disasm:
//   push ecx                 ; reserve 1 dword local (byte buffer)
//   push esi ; mov esi,ecx   ; save esi, esi = this
//   mov edx,[esi+4]          ; edx = this->m_status
//   mov ecx,[esi]            ; ecx = this->m_reader
//   lea eax,[esp+7]          ; &localByte (high byte of reserved dword)
//   push eax
//   call Reader_Notify       ; __fastcall(reader, status, &localByte)
//   mov esi,[esi]            ; r = this->m_reader
//   test esi,esi ; je +      ; if (r)
//   push esi ; call Release ; pop ecx   ; __cdecl release
//   pop esi ; pop ecx ; ret

struct CActiveFileReader;

struct CActiveFile
{
    CActiveFileReader* m_reader; // +0x00
    fable_i32          m_status; // +0x04
};

// __fastcall member on the reader, modeled as free __fastcall so edx carries m_status.
extern void FABLE_FASTCALL CActiveFileReader_Notify(CActiveFileReader* thisptr,
                                                    fable_i32 status,
                                                    fable_u8* outByte);

// free release helper (single stack arg, __cdecl)
extern void FABLE_CDECL CActiveFile_ReleaseReader(CActiveFileReader* reader);

void FABLE_FASTCALL CActiveFile_OnReadFinished_471df4(CActiveFile* thisptr)
{
    fable_u8 localByte;

    CActiveFileReader_Notify(thisptr->m_reader, thisptr->m_status, &localByte);

    CActiveFileReader* r = thisptr->m_reader;
    if (r)
        CActiveFile_ReleaseReader(r);
}