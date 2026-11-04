#include "fable_threaded_file.h"

// CCountedPointer<CThreadedFile>::Reset(CThreadedFile*) @ 0x009A9C80.
void CCountedPointer<CThreadedFile>::Reset(CThreadedFile* data)
{
    if (control_ != 0)
    {
        --control_->RefCount;
        if (control_->RefCount == 0)
        {
            control_->DeleteFunc(control_->Data);
            delete control_;
        }
    }

    control_ = 0;
    data_ = data;
    if (data_ != 0)
    {
        control_ = new CCPPointerInfo(&DeleteData, data_);
    }
}
