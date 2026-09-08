struct CCreatureActionBase {};
struct CCreatureAction_DragonBurninateInto : CCreatureActionBase {
    unsigned char State[0x14c];
    CCreatureAction_DragonBurninateInto(const CCreatureAction_DragonBurninateInto& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_DragonBurninateInto::Clone() const { return new CCreatureAction_DragonBurninateInto(*this); }