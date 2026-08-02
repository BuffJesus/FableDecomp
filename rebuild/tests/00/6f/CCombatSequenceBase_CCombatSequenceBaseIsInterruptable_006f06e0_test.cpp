struct CCombatSequenceBaseVtbl006f06e0;

struct CCombatSequenceBase006f06e0
{
    CCombatSequenceBaseVtbl006f06e0* vtbl;
};

typedef int (__fastcall *IsInterruptableImplFn006f06e0)(CCombatSequenceBase006f06e0* self);

struct CCombatSequenceBaseVtbl006f06e0
{
    void* slot0;
    void* slot1;
    void* slot2;
    void* slot3;
    IsInterruptableImplFn006f06e0 slot4;
};

bool __fastcall CCombatSequenceBase_IsInterruptable(CCombatSequenceBase006f06e0* self, void* edx)
{
    return self->vtbl->slot4(self) != 0;
}

static int __fastcall ReturnFive(CCombatSequenceBase006f06e0* self)
{
    return 5;
}

static int __fastcall ReturnZero(CCombatSequenceBase006f06e0* self)
{
    return 0;
}

#include <stdio.h>

int main()
{
    CCombatSequenceBaseVtbl006f06e0 vtblTrue;
    vtblTrue.slot4 = ReturnFive;
    CCombatSequenceBase006f06e0 objTrue;
    objTrue.vtbl = &vtblTrue;

    CCombatSequenceBaseVtbl006f06e0 vtblFalse;
    vtblFalse.slot4 = ReturnZero;
    CCombatSequenceBase006f06e0 objFalse;
    objFalse.vtbl = &vtblFalse;

    bool r1 = CCombatSequenceBase_IsInterruptable(&objTrue, 0);
    bool r2 = CCombatSequenceBase_IsInterruptable(&objFalse, 0);

    if (r1 == true && r2 == false)
    {
        printf("PASS_006f06e0_marker\n");
    }
    else
    {
        printf("FAIL r1=%d r2=%d\n", (int)r1, (int)r2);
    }
    return 0;
}