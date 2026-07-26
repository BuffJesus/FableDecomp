struct CCreatureActionBase { int vtbl; };

bool __fastcall CCreatureActionBase_IsStrikeAction(const CCreatureActionBase* self)
{
    (void)self;
    return false;
}