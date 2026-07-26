#include "fable_filesystem.h"

#include <stdlib.h>

namespace
{
    struct CWideStringStorage
    {
        wchar_t* text;
        fable_u32 unknown04;
        fable_u32 unknown08;
        fable_i32 owners;
    };
}

CWideString::~CWideString()
{
    CWideStringStorage* storage =
        reinterpret_cast<CWideStringStorage*>(storage_);

    if (storage != 0)
    {
        --storage->owners;
        if (storage->owners <= 0)
        {
            storage = reinterpret_cast<CWideStringStorage*>(storage_);
            if (storage != 0)
            {
                if (storage->text != 0)
                    free(storage->text);
                delete storage;
            }
        }
        storage_ = 0;
    }

    --g_CStringInstanceCount_013BCA20;
}
