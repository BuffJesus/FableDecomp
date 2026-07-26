struct CCombatAbilityStrafeDef;

extern void __fastcall CCombatAbilityStrafeDef_Copy_impl(CCombatAbilityStrafeDef* self, void* dummy, const CCombatAbilityStrafeDef* other);

void __fastcall CCombatAbilityStrafeDef_Copy(CCombatAbilityStrafeDef* self, void* dummy, const CCombatAbilityStrafeDef* other)
{
    CCombatAbilityStrafeDef_Copy_impl(self, dummy, other);
}