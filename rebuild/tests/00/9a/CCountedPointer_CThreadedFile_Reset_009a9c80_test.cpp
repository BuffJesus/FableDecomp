#include <stdio.h>
#include <string.h>

#include "fable_threaded_file.h"

namespace
{
    fable_u8 g_newControl[sizeof(CCPPointerInfo)];
    bool g_failAllocation;
    unsigned int g_allocateCalls;
    size_t g_allocateSize;
    unsigned int g_controlDeleteCalls;
    unsigned int g_objectDeleteCalls;
    void* g_deletedControl;
    void* g_deletedObject;

    void ResetObservations()
    {
        memset(g_newControl, 0, sizeof(g_newControl));
        g_failAllocation = false;
        g_allocateCalls = 0;
        g_allocateSize = 0;
        g_controlDeleteCalls = 0;
        g_objectDeleteCalls = 0;
        g_deletedControl = 0;
        g_deletedObject = 0;
    }

    CCPPointerInfo* NewControl()
    {
        return reinterpret_cast<CCPPointerInfo*>(g_newControl);
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
        CCPPointerInfo oldControl(
            &CCountedPointer<CThreadedFile>::DeleteData, oldObject);
        oldControl.RefCount = 2;
        CCountedPointer<CThreadedFile> counted;
        counted.data_ = oldObject;
        counted.control_ = &oldControl;

        counted.Reset(newObject);

        return
            oldControl.RefCount == 1 &&
            g_objectDeleteCalls == 0 &&
            g_controlDeleteCalls == 0 &&
            g_allocateCalls == 1 &&
            g_allocateSize == sizeof(CCPPointerInfo) &&
            counted.data_ == newObject &&
            counted.control_ == NewControl() &&
            NewControl()->RefCount == 1 &&
            NewControl()->Data == newObject &&
            NewControl()->DeleteFunc ==
                &CCountedPointer<CThreadedFile>::DeleteData;
    }

    bool CheckLastOwnerRelease()
    {
        ResetObservations();
        fable_u8 oldBytes[sizeof(CThreadedFile)];
        CThreadedFile* oldObject =
            reinterpret_cast<CThreadedFile*>(oldBytes);
        CCPPointerInfo oldControl(
            &CCountedPointer<CThreadedFile>::DeleteData, oldObject);
        CCountedPointer<CThreadedFile> counted;
        counted.data_ = oldObject;
        counted.control_ = &oldControl;

        counted.Reset(0);

        return
            oldControl.RefCount == 0 &&
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

// Engine allocation returns null on failure; the fixture records every call.
void* operator new(size_t size)
{
    ++g_allocateCalls;
    g_allocateSize = size;
    if (size > sizeof(g_newControl) || g_failAllocation)
    {
        return 0;
    }
    return g_newControl;
}

void operator delete(void* pointer)
{
    ++g_controlDeleteCalls;
    g_deletedControl = pointer;
}

void FABLE_FASTCALL CCountedPointer<CThreadedFile>::DeleteData(void* data)
{
    ++g_objectDeleteCalls;
    g_deletedObject = data;
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
