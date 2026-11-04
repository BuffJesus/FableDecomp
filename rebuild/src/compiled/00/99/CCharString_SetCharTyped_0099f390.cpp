#include "fable_string.h"

void CCharString::SetChar(long index, char character)
{
    if (storage_)
    {
        MakeOwnCopyOfData();
        storage_->Data.PData[index] = character;
    }
}
