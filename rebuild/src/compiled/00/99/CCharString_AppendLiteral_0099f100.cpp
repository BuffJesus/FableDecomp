#include "fable_string.h"
#include <string.h>

const CCharString& CCharString::operator+=(const char* text)
{
    if (storage_ != 0)
    {
        MakeOwnCopyOfData();
        CCharStringData* data = storage_;
        long length = 0;
        if (text[0] != '\0')
        {
            do
                ++length;
            while (text[length] != '\0');
        }
        data->Data.Append(text, length);
        return *this;
    }
    if (text != 0 && text[0] != '\0')
        storage_ = new CCharStringData(text);
    return *this;
}
