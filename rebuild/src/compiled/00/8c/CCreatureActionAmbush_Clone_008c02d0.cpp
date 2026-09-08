struct CCreatureActionBase {};
struct CCreatureAction_Ambush : CCreatureActionBase {
    unsigned char State[0x120];
    CCreatureAction_Ambush(const CCreatureAction_Ambush& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_Ambush::Clone() const { return new CCreatureAction_Ambush(*this); }