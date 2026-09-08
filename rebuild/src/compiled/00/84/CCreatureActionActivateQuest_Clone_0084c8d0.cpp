struct CCreatureActionBase {};
struct CCreatureAction_ActivateQuest : CCreatureActionBase {
    unsigned char State[0xb8];
    CCreatureAction_ActivateQuest(const CCreatureAction_ActivateQuest& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_ActivateQuest::Clone() const { return new CCreatureAction_ActivateQuest(*this); }