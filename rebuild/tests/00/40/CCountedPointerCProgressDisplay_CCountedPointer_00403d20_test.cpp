#include <stdio.h>
#include <string.h>

#include "rebuild_abi.h"

struct FableProgressDisplayControl_00403D20
{
    fable_i32 owners;
    void (FABLE_FASTCALL *destroyObject)(void*, void*);
    const void* object;
};

struct FableProgressDisplayCountedPointer_00403D20
{
    const void* object;
    FableProgressDisplayControl_00403D20* control;
};

extern "C" FableProgressDisplayCountedPointer_00403D20* FABLE_FASTCALL
FableProgressDisplayCountedPointerCtor_00403D20(
    FableProgressDisplayCountedPointer_00403D20* counted,
    void*,
    const void* object);
extern "C" void FABLE_FASTCALL
FableProgressDisplayObjectDelete_00403600(void* object, void*);

namespace
{
    FableProgressDisplayControl_00403D20 g_newControl;
    bool g_failAllocation;
    unsigned int g_allocateCalls;

    void ResetObservations()
    {
        memset(&g_newControl, 0, sizeof(g_newControl));
        g_failAllocation = false;
        g_allocateCalls = 0;
    }

    bool CheckObjectConstruction()
    {
        ResetObservations();
        int object;
        FableProgressDisplayCountedPointer_00403D20 counted;
        counted.object = (const void*)0xdeadbeef;
        counted.control = (FableProgressDisplayControl_00403D20*)0xdeadbeef;

        FableProgressDisplayCountedPointer_00403D20* result =
            FableProgressDisplayCountedPointerCtor_00403D20(
                &counted,
                0,
                &object);

        return
            result == &counted &&
            g_allocateCalls == 1 &&
            counted.object == &object &&
            counted.control == &g_newControl &&
            g_newControl.owners == 1 &&
            g_newControl.object == &object &&
            g_newControl.destroyObject ==
                &FableProgressDisplayObjectDelete_00403600;
    }

    bool CheckNullObject()
    {
        ResetObservations();
        FableProgressDisplayCountedPointer_00403D20 counted;
        counted.object = (const void*)0xdeadbeef;
        counted.control = (FableProgressDisplayControl_00403D20*)0xdeadbeef;

        FableProgressDisplayCountedPointer_00403D20* result =
            FableProgressDisplayCountedPointerCtor_00403D20(
                &counted,
                0,
                0);

        return
            result == &counted &&
            g_allocateCalls == 0 &&
            counted.object == 0 &&
            counted.control == 0;
    }

    bool CheckAllocationFailure()
    {
        ResetObservations();
        g_failAllocation = true;
        int object;
        FableProgressDisplayCountedPointer_00403D20 counted;
        counted.object = (const void*)0xdeadbeef;
        counted.control = (FableProgressDisplayControl_00403D20*)0xdeadbeef;

        FableProgressDisplayCountedPointer_00403D20* result =
            FableProgressDisplayCountedPointerCtor_00403D20(
                &counted,
                0,
                &object);

        return
            result == &counted &&
            g_allocateCalls == 1 &&
            counted.object == &object &&
            counted.control == 0;
    }
}

extern "C" void* FABLE_CDECL
FableProgressDisplayControlAllocate_00403D20(fable_u32 size)
{
    ++g_allocateCalls;
    if (size != sizeof(g_newControl) || g_failAllocation)
    {
        return 0;
    }
    return &g_newControl;
}

extern "C" void FABLE_FASTCALL
FableProgressDisplayObjectDelete_00403600(void* object, void*)
{
    (void)object;
}

int main()
{
    const bool passed =
        CheckObjectConstruction() &&
        CheckNullObject() &&
        CheckAllocationFailure();
    if (!passed)
    {
        printf("FABLETLC_PROGRESS_DISPLAY_COUNTED_CTOR FAIL\n");
        return 1;
    }

    printf("FABLETLC_PROGRESS_DISPLAY_COUNTED_CTOR PASS\n");
    return 0;
}