#include "rebuild_abi.h"
// CChunkedFileChunk::~CChunkedFileChunk @ 0x00c10a10
// [ecx]=vtbl; jmp base_dtor
extern void *FableVtbl_CChunkedFileChunk_c10a10[];
struct CChunkedFileChunk { void *vptr; void Dtor(); };
extern "C" void FABLE_FASTCALL FableBaseDtor_c10a10(CChunkedFileChunk *self);
void CChunkedFileChunk::Dtor()
{
    vptr = (void*)&FableVtbl_CChunkedFileChunk_c10a10;
    FableBaseDtor_c10a10(this);
}