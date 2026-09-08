struct CCreatureActionBase {};
struct CCreatureAction_MagicBlast : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_MagicBlast(const CCreatureAction_MagicBlast& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_MagicBlast::Clone() const { return new CCreatureAction_MagicBlast(*this); }