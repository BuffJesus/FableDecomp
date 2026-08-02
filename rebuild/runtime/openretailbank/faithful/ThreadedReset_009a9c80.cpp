#include "fable_threaded_file.h"

struct FableThreadedFileControl_009A9C80
{
    fable_i32 owners;
    void (FABLE_FASTCALL *destroyObject)(void*);
    void* object;
};

extern "C" void* FABLE_CDECL
FableThreadedFileControlAllocate_009A9C80(fable_u32 size);
extern "C" void FABLE_CDECL
FableThreadedFileControlDelete_009A9C80(void* control);
extern "C" void FABLE_FASTCALL
FableThreadedFileObjectDelete_009A9040(void* object);

// CCountedPointer<CThreadedFile>::Reset(CThreadedFile*) @ 0x009A9C80.
void CCountedPointer<CThreadedFile>::Reset(CThreadedFile* data)
{
    FableThreadedFileControl_009A9C80* oldControl =
        reinterpret_cast<FableThreadedFileControl_009A9C80*>(control_);
    if (oldControl != 0)
    {
        --oldControl->owners;
        oldControl =
            reinterpret_cast<FableThreadedFileControl_009A9C80*>(control_);
        if (oldControl->owners == 0)
        {
            oldControl->destroyObject(oldControl->object);
            FableThreadedFileControlDelete_009A9C80(control_);
        }
    }

    control_ = 0;
    data_ = data;
    if (data != 0)
    {
        void* newControlVoid =
            FableThreadedFileControlAllocate_009A9C80(
                sizeof(FableThreadedFileControl_009A9C80));
        FableThreadedFileControl_009A9C80* newControl =
            reinterpret_cast<FableThreadedFileControl_009A9C80*>(newControlVoid);
        if (newControlVoid != 0)
        {
            CThreadedFile* object = data_;
            newControl->owners = 1;
            newControl->destroyObject = &FableThreadedFileObjectDelete_009A9040;
            newControl->object = object;
            control_ = newControl;
        }
        else
        {
            control_ = newControlVoid;
        }
    }
}
