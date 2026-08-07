#include "rebuild_abi.h"

// CFileDataInputStream::NeedsBufferedRead @ 0x009943b0
// __fastcall: returns (requested < this->m_threshold at +0x20), signed-less.
// One stack arg, ret 4.
struct FableStream_009943b0
{
    bool NeedsBufferedRead(int requested);
};

bool FableStream_009943b0::NeedsBufferedRead(int requested)
{
    int threshold = *(const int*)((const char*)this + 0x20);
    return requested < threshold;
}