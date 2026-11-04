// CCharString::MakeOwnCopyOfData() @ 0x0099EAF0 (FableWin
// ?MakeOwnCopyOfData@CCharString@@AAEXXZ; called before a write, e.g. by
// operator+= 0x0099F0A0).
#include "fable_string.h"

// Shared data gets a private one-owner copy of its text before a write.
void CCharString::MakeOwnCopyOfData()
{
    if (storage_ != 0 && storage_->owners > 1)
    {
        const char* text = storage_->Data.PData;
        CCharStringData* copy = new CCharStringData(text);
        UnassignString();
        storage_ = copy;
    }
}