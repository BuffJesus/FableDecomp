#include "fable_boot.h"

#include <new>
#include <stdlib.h>

fable_i32 g_CWideStringInstanceCount_013BCA20 = 0;
fable_i32 g_CCharStringInstanceCount_013BD800 = 0;

void FABLE_FASTCALL FableConstructWideString(CWideString* value)
{
    ::new (value) CWideString;
}

void FABLE_FASTCALL FableConstructCharString(CCharString* value)
{
    ::new (value) CCharString;
}

void CCharString::UnassignString()
{
    if (storage_ == 0)
        return;

    --storage_->owners;
    if (storage_->owners <= 0)
    {
        free(storage_->text);
        free(storage_);
    }
    storage_ = 0;
}
