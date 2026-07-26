struct CCombatActionBase {
    char pad[0xc0];
    int strikeCount;
};

bool __fastcall CCombatActionBase_IsStrikeAction(const CCombatActionBase* self)
{
    return self->strikeCount > 0;
}