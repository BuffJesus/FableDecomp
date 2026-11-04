#include "fable_filesystem.h"

#include <stdlib.h>

// Retail's destructor is inline: every unit that destroys a CWideString
// emits it as a pick-any COMDAT (the operator+ units inline it and emit one
// too), and the linker kept this copy. Defining it inline here as well lets
// these objects link together; the use below (link scaffolding, not retail
// code, kept a call by noinline) makes this unit emit it.
inline __declspec(noinline) CWideString::~CWideString()
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

namespace
{
    void EmitDestructor(void* text)
    {
        static_cast<CWideString*>(text)->CWideString::~CWideString();
    }
}
