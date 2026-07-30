#include <stdio.h>
#include <string.h>

#include "fable_threaded_file.h"

struct FableThreadedFileControl_009A9C80
{
    fable_i32 owners;
    void (FABLE_FASTCALL *destroyObject)(void*, void*);
    void* object;
};

extern "C" void FABLE_FASTCALL
FableThreadedFileObjectDelete_009A9040(void* object, void*);

namespace
{
    FableThreadedFileControl_009A9C80 g_newControl;
    bool g_failAllocation;
    unsigned int g_allocateCalls;
    unsigned int g_controlDeleteCalls;
    unsigned int g_objectDeleteCalls;
    void* g_deletedControl;
    void* g_deletedObject;

    void ResetObservations()
    {
        memset(&g_newControl, 0, sizeof(g_newControl));
        g_failAllocation = false;
        g_allocateCalls = 0;
        g_controlDeleteCalls = 0;
        g_objectDeleteCalls = 0;
        g_deletedControl = 0;
        g_deletedObject = 0;
    }

    bool CheckSharedOwnerReplacement()
    {
        ResetObservations();
        fable_u8 oldBytes[sizeof(CThreadedFile)];
        fable_u8 newBytes[sizeof(CThreadedFile)];
        CThreadedFile* oldObject =
            reinterpret_cast<CThreadedFile*>(oldBytes);
        CThreadedFile* newObject =
            reinterpret_cast<CThreadedFile*>(newBytes);
        FableThreadedFileControl_009A9C80 oldControl =
        {
            2,
            &FableThreadedFileObjectDelete_009A9040,
            oldObject
        };
        CCountedPointer<CThreadedFile> counted;
        counted.data_ = oldObject;
        counted.control_ = &oldControl;

        counted.Reset(newObject);

        return
            oldControl.owners == 1 &&
            g_objectDeleteCalls == 0 &&
            g_controlDeleteCalls == 0 &&
            g_allocateCalls == 1 &&
            counted.data_ == newObject &&
            counted.control_ == &g_newControl &&
            g_newControl.owners == 1 &&
            g_newControl.object == newObject &&
            g_newControl.destroyObject ==
                &FableThreadedFileObjectDelete_009A9040;
    }

    bool CheckLastOwnerRelease()
    {
        ResetObservations();
        fable_u8 oldBytes[sizeof(CThreadedFile)];
        CThreadedFile* oldObject =
            reinterpret_cast<CThreadedFile*>(oldBytes);
        FableThreadedFileControl_009A9C80 oldControl =
        {
            1,
            &FableThreadedFileObjectDelete_009A9040,
            oldObject
        };
        CCountedPointer<CThreadedFile> counted;
        counted.data_ = oldObject;
        counted.control_ = &oldControl;

        counted.Reset(0);

        return
            oldControl.owners == 0 &&
            g_objectDeleteCalls == 1 &&
            g_deletedObject == oldObject &&
            g_controlDeleteCalls == 1 &&
            g_deletedControl == &oldControl &&
            g_allocateCalls == 0 &&
            counted.data_ == 0 &&
            counted.control_ == 0;
    }

    bool CheckAllocationFailure()
    {
        ResetObservations();
        g_failAllocation = true;
        fable_u8 objectBytes[sizeof(CThreadedFile)];
        CThreadedFile* object =
            reinterpret_cast<CThreadedFile*>(objectBytes);
        CCountedPointer<CThreadedFile> counted;
        counted.data_ = 0;
        counted.control_ = 0;

        counted.Reset(object);

        return
            g_allocateCalls == 1 &&
            counted.data_ == object &&
            counted.control_ == 0;
    }
}

extern "C" void* FABLE_CDECL
FableThreadedFileControlAllocate_009A9C80(fable_u32 size)
{
    ++g_allocateCalls;
    if (size != sizeof(g_newControl) || g_failAllocation)
    {
        return 0;
    }
    return &g_newControl;
}

extern "C" void FABLE_CDECL
FableThreadedFileControlDelete_009A9C80(void* control)
{
    ++g_controlDeleteCalls;
    g_deletedControl = control;
}

extern "C" void FABLE_FASTCALL
FableThreadedFileObjectDelete_009A9040(
    void* object,
    void*)
{
    ++g_objectDeleteCalls;
    g_deletedObject = object;
}

int main()
{
    const bool passed =
        CheckSharedOwnerReplacement() &&
        CheckLastOwnerRelease() &&
        CheckAllocationFailure();
    if (!passed)
    {
        printf("FABLETLC_THREADED_FILE_COUNTED_RESET FAIL\n");
        return 1;
    }

    printf("FABLETLC_THREADED_FILE_COUNTED_RESET PASS\n");
    return 0;
}
