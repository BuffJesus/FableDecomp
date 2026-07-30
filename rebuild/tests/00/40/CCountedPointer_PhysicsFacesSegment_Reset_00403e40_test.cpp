#include <stdio.h>
#include <string.h>

#include "rebuild_abi.h"

struct FablePhysicsFacesControl_00403E40
{
    fable_i32 owners;
    void (FABLE_FASTCALL *destroyObject)(void*, void*);
    const void* object;
};

struct FablePhysicsFacesCountedPointer_00403E40
{
    const void* object;
    FablePhysicsFacesControl_00403E40* control;
};

extern "C" void FABLE_FASTCALL
FablePhysicsFacesCountedPointerReset_00403E40(
    FablePhysicsFacesCountedPointer_00403E40* counted,
    void*,
    const void* object);
extern "C" void FABLE_FASTCALL
FablePhysicsFacesObjectDelete_00403650(
    void* object,
    void*);

namespace
{
    FablePhysicsFacesControl_00403E40 g_newControl;
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
        int oldObject;
        int newObject;
        FablePhysicsFacesControl_00403E40 oldControl =
        {
            2,
            &FablePhysicsFacesObjectDelete_00403650,
            &oldObject
        };
        FablePhysicsFacesCountedPointer_00403E40 counted =
        {
            &oldObject,
            &oldControl
        };

        FablePhysicsFacesCountedPointerReset_00403E40(
            &counted,
            0,
            &newObject);

        return
            oldControl.owners == 1 &&
            g_objectDeleteCalls == 0 &&
            g_controlDeleteCalls == 0 &&
            g_allocateCalls == 1 &&
            counted.object == &newObject &&
            counted.control == &g_newControl &&
            g_newControl.owners == 1 &&
            g_newControl.object == &newObject &&
            g_newControl.destroyObject ==
                &FablePhysicsFacesObjectDelete_00403650;
    }

    bool CheckLastOwnerRelease()
    {
        ResetObservations();
        int oldObject;
        FablePhysicsFacesControl_00403E40 oldControl =
        {
            1,
            &FablePhysicsFacesObjectDelete_00403650,
            &oldObject
        };
        FablePhysicsFacesCountedPointer_00403E40 counted =
        {
            &oldObject,
            &oldControl
        };

        FablePhysicsFacesCountedPointerReset_00403E40(
            &counted,
            0,
            0);

        return
            oldControl.owners == 0 &&
            g_objectDeleteCalls == 1 &&
            g_deletedObject == &oldObject &&
            g_controlDeleteCalls == 1 &&
            g_deletedControl == &oldControl &&
            g_allocateCalls == 0 &&
            counted.object == 0 &&
            counted.control == 0;
    }

    bool CheckAllocationFailure()
    {
        ResetObservations();
        g_failAllocation = true;
        int object;
        FablePhysicsFacesCountedPointer_00403E40 counted = {0, 0};

        FablePhysicsFacesCountedPointerReset_00403E40(
            &counted,
            0,
            &object);

        return
            g_allocateCalls == 1 &&
            counted.object == &object &&
            counted.control == 0;
    }
}

extern "C" void* FABLE_CDECL
FablePhysicsFacesControlAllocate_00403E40(fable_u32 size)
{
    ++g_allocateCalls;
    if (size != sizeof(g_newControl) || g_failAllocation)
    {
        return 0;
    }
    return &g_newControl;
}

extern "C" void FABLE_CDECL
FablePhysicsFacesControlDelete_00403E40(void* control)
{
    ++g_controlDeleteCalls;
    g_deletedControl = control;
}

extern "C" void FABLE_FASTCALL
FablePhysicsFacesObjectDelete_00403650(
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
        printf("FABLETLC_PHYSICS_FACES_COUNTED_RESET FAIL\n");
        return 1;
    }

    printf("FABLETLC_PHYSICS_FACES_COUNTED_RESET PASS\n");
    return 0;
}
