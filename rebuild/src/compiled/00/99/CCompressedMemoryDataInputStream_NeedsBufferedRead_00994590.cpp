#include "rebuild_abi.h"

// CCompressedMemoryDataInputStream::NeedsBufferedRead @ 0x00994590
// __fastcall: returns (requested < this->m_threshold at +0x1c), signed-less.
// One stack arg, ret 4.
struct FableStream_00994590
{
    bool NeedsBufferedRead(int requested);
};

bool FableStream_00994590::NeedsBufferedRead(int requested)
{
    int threshold = *(const int*)((const char*)this + 0x1c);
    return requested < threshold;
}