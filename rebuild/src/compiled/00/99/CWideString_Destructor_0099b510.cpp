#include "fable_filesystem.h"

#include <stdlib.h>

CWideString::~CWideString()
{
    CWideStringData* storage = storage_;

    if (storage != 0)
    {
        --storage->owners;
        if (storage->owners <= 0)
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

    --g_CWideStringInstanceCount_013BCA20;
}