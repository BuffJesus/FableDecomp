struct CCombatSequenceBase_vtbl
{
    void* slots[16];
};

struct CCombatSequenceBase
{
    CCombatSequenceBase_vtbl* vtbl;
};

typedef unsigned int (__fastcall *IsAllowedOnOuterRing_fn)(CCombatSequenceBase* self, void* edx);

unsigned int __fastcall CCombatSequenceBase_IsAllowedOnOuterRing(CCombatSequenceBase* self, void* edx)
{
    return (((IsAllowedOnOuterRing_fn)(*(void***)self)[0xF])(self, edx) >> 7) & 1;
}