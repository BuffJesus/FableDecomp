#pragma optimize("s",on)
#include "rebuild_abi.h"

// CActiveFile::OnReadFinished @ 0x0046f68c
//
// thisptr (esi):
//   +0x00  Reader*  m_reader   (object with a __fastcall member taking edx + &byte)
//   +0x04  fable_i32 m_status   (passed in edx to the member call -> __fastcall 2nd reg)
//
// Sequence:
//   edx = this->m_status;
//   ecx = this->m_reader;
//   Reader_Notify(ecx, edx, &localByte);      // __fastcall(obj, status, char*)
//   r = this->m_reader;
//   if (r) ReleaseReader(r);

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

void FABLE_FASTCALL CActiveFile_OnReadFinished(CActiveFile* thisptr)
{
    fable_u8 localByte;

    CActiveFileReader_Notify(thisptr->m_reader, thisptr->m_status, &localByte);

    CActiveFileReader* r = thisptr->m_reader;
    if (r)
        CActiveFile_ReleaseReader(r);
}