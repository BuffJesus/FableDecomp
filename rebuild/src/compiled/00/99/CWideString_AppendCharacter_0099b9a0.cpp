#include "fable_string.h"

const CWideString& CWideString::operator+=(wchar_t character)
{
    if (storage_ != 0)
    {
        MakeOwnCopyOfData();
        storage_->Data.append(1, character);
    }
    else
    {
        wchar_t text[2];
        text[0] = character;
        text[1] = 0;
        if (text[0] != 0)
            storage_ = AllocStringData(text, -1);
    }
    return *this;
}
