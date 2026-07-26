typedef unsigned long ulong;
struct CCombatActionBase { void* vtbl; };
ulong __fastcall CCombatActionBase_GetAnimExtraFlags(const CCombatActionBase* self)
{
    (void)self;
    return 8u;
}