#include "fable_string.h"

// lib_string.cpp, retail 0099EB40 (FableWin
// ?MakeOwnCopyOfDataOrEmptyString@CCharString@@IAEXXZ): before a write, take a
// private copy of shared data, or start from an empty block. MakeOwnCopyOfData
// (out of line at 0x0099EAF0) is expanded here.

__forceinline void CCharString::MakeOwnCopyOfData()
{
    if (storage_ != 0 && storage_->owners > 1)
    {
        const char* text = storage_->Data.PData;
        CCharStringData* copy = new CCharStringData(text);
        UnassignString();
        storage_ = copy;
    }
}

void CCharString::MakeOwnCopyOfDataOrEmptyString()
{
    if (storage_ != 0)
        MakeOwnCopyOfData();
    else
        storage_ = new CCharStringData(0, ' ');
}
