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
    IsInterruptableImplFn006f06e0 slot4; // vtbl+0x10
};

bool __fastcall CCombatSequenceBase_IsInterruptable(CCombatSequenceBase006f06e0* self, void* edx)
{
    return self->vtbl->slot4(self) != 0;
}