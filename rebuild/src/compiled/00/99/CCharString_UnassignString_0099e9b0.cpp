// CCharString::UnassignString @ 0x0099E9B0 (FableWin ?UnassignString@CCharString@@AAEXXZ).
//
// Drops this string's reference on its shared data. The last owner releases
// the CBasicString<char> buffer (delete[]), resets its length and the 31-bit
// capacity (keeping the UseFastExtend top bit), then deletes the data block.

#include "fable_string.h"

// Retail calls MSVCR71 ??_V (operator delete[]) for the buffer: VC7.1 only
// routes delete[] there when the array form is declared.
#include <new>

void CCharString::UnassignString()
{
    CCharStringData* shared = storage_;
    if (shared != 0)
    {
        if (--shared->owners <= 0)
        {
            CCharStringData* storage = storage_;
            if (storage != 0)
            {
                if (storage->Data.PData != 0)
                {
                    delete[] storage->Data.PData;
                    storage->Data.PData = 0;
                }
                storage->Data.StringLength = 0;
                storage->Data.DataLength = 0;
                delete storage;
            }
        }
        storage_ = 0;
    }
}
