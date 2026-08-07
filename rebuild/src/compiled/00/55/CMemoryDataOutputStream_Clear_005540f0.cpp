#include "rebuild_abi.h"
// CMemoryDataOutputStream::Clear @ 0x005540f0
// push esi; mov esi,ecx; call base; mov [esi+0x198],0; pop esi; ret
// Reset the base stream, then clear the length field.
struct CMemoryDataOutputStream;
extern "C" void FABLE_FASTCALL FableStreamBaseClear_5540f0(CMemoryDataOutputStream *self);
struct CMemoryDataOutputStream {
    char pad00[0x198];
    long m_length;   // +0x198
    void Clear();
};
void CMemoryDataOutputStream::Clear()
{
    FableStreamBaseClear_5540f0(this);
    m_length = 0;
}