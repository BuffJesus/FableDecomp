struct CCreatureActionBase {};
struct CCreatureAction_DragonTakeOffIntoFlying : CCreatureActionBase {
    unsigned char State[0x150];
    CCreatureAction_DragonTakeOffIntoFlying(const CCreatureAction_DragonTakeOffIntoFlying& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_DragonTakeOffIntoFlying::Clone() const { return new CCreatureAction_DragonTakeOffIntoFlying(*this); }