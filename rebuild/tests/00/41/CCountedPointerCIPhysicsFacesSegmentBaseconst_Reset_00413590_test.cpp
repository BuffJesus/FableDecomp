#include <stdio.h>
#include <string.h>

#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_FASTCALL __fastcall
#else
#  define FABLE_CDECL
#  define FABLE_FASTCALL
#endif

typedef signed int fable_i32;
typedef unsigned int fable_u32;

typedef void(FABLE_FASTCALL *FableFacesDestroyObject_00413590)(void *object);

struct FableFacesControl_00413590
{
    fable_i32 owners;
    FableFacesDestroyObject_00413590 destroyObject;
    void *object;
};

struct FableFacesCountedPointer_00413590
{
    void *object;
    FableFacesControl_00413590 *control;
};

extern "C" void FABLE_FASTCALL FableFacesCountedPointerReset_00413590(
    FableFacesCountedPointer_00413590 *self,
    int edxDummy,
    void *newObject);

extern void FABLE_FASTCALL FableFacesObjectDelete_00413300(void *object);
extern "C" void *FABLE_CDECL FableFacesControlAllocate_00413590(fable_u32 size);
extern "C" void FABLE_CDECL FableFacesControlDelete_00413590(void *control);

/* ---- masked-callee observation state ---- */
static FableFacesControl_00413590 g_newControl;
static bool g_failAllocation;
static unsigned int g_allocateCalls;
static unsigned int g_allocateSize;
static unsigned int g_controlDeleteCalls;
static void *g_deletedControl;
static unsigned int g_objectDeleteCalls;
static void *g_deletedObject;

static void ResetObservations()
{
    memset(&g_newControl, 0, sizeof(g_newControl));
    g_failAllocation = false;
    g_allocateCalls = 0;
    g_allocateSize = 0;
    g_controlDeleteCalls = 0;
    g_deletedControl = 0;
    g_objectDeleteCalls = 0;
    g_deletedObject = 0;
}

void FABLE_FASTCALL FableFacesObjectDelete_00413300(void *object)
{
    ++g_objectDeleteCalls;
    g_deletedObject = object;
}

extern "C" void *FABLE_CDECL FableFacesControlAllocate_00413590(fable_u32 size)
{
    ++g_allocateCalls;
    g_allocateSize = size;
    if (g_failAllocation)
        return 0;
    return &g_newControl;
}

extern "C" void FABLE_CDECL FableFacesControlDelete_00413590(void *control)
{
    ++g_controlDeleteCalls;
    g_deletedControl = control;
}

/* ---- scenarios ---- */
static bool CheckSharedOwnerReplacement()
{
    ResetObservations();
    int oldObject;
    int newObject;
    FableFacesControl_00413590 oldControl;
    oldControl.owners = 2;
    oldControl.destroyObject = &FableFacesObjectDelete_00413300;
    oldControl.object = &oldObject;

    FableFacesCountedPointer_00413590 counted;
    counted.object = &oldObject;
    counted.control = &oldControl;

    FableFacesCountedPointerReset_00413590(&counted, 0, &newObject);

    return oldControl.owners == 1 &&
           g_objectDeleteCalls == 0 &&
           g_controlDeleteCalls == 0 &&
           g_allocateCalls == 1 &&
           g_allocateSize == 0xc &&
           counted.object == &newObject &&
           counted.control == &g_newControl &&
           g_newControl.owners == 1 &&
           g_newControl.object == &newObject &&
           g_newControl.destroyObject == &FableFacesObjectDelete_00413300;
}

static bool CheckLastOwnerRelease()
{
    ResetObservations();
    int oldObject;
    FableFacesControl_00413590 oldControl;
    oldControl.owners = 1;
    oldControl.destroyObject = &FableFacesObjectDelete_00413300;
    oldControl.object = &oldObject;

    FableFacesCountedPointer_00413590 counted;
    counted.object = &oldObject;
    counted.control = &oldControl;

    FableFacesCountedPointerReset_00413590(&counted, 0, 0);

    return oldControl.owners == 0 &&
           g_objectDeleteCalls == 1 &&
           g_deletedObject == &oldObject &&
           g_controlDeleteCalls == 1 &&
           g_deletedControl == &oldControl &&
           g_allocateCalls == 0 &&
           counted.object == 0 &&
           counted.control == 0;
}

static bool CheckAllocationFailure()
{
    ResetObservations();
    g_failAllocation = true;
    int object;
    FableFacesCountedPointer_00413590 counted;
    counted.object = 0;
    counted.control = 0;

    FableFacesCountedPointerReset_00413590(&counted, 0, &object);

    return g_allocateCalls == 1 &&
           g_allocateSize == 0xc &&
           counted.object == &object &&
           counted.control == 0;
}

int main()
{
    bool passed = CheckSharedOwnerReplacement() &&
                  CheckLastOwnerRelease() &&
                  CheckAllocationFailure();
    if (!passed)
    {
        printf("FABLETLC_PHYSICS_FACES_COUNTED_RESET_413590 FAIL\n");
        return 1;
    }
    printf("FABLETLC_PHYSICS_FACES_COUNTED_RESET_413590 PASS\n");
    return 0;
}