struct CCreatureActionBase {};
struct CCreatureAction_DragonSwoop : CCreatureActionBase {
    unsigned char State[0x138];
    CCreatureAction_DragonSwoop(const CCreatureAction_DragonSwoop& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_DragonSwoop::Clone() const { return new CCreatureAction_DragonSwoop(*this); }