#include "fable_string.h"

#include <stdlib.h>

const CWideString& CWideString::operator=(const wchar_t* text)
{
    CWideStringData* storage = storage_;
    if (storage != 0)
    {
        if (--storage->owners <= 0)
        {
            storage = storage_;
            if (storage != 0)
            {
                if (storage->text != 0)
                    free(storage->text);
                delete storage;
            }
        }
        storage_ = 0;
    }

    if (text != 0 && *text != L'\0')
        storage_ = AllocStringData(text, -1);

    return *this;
}