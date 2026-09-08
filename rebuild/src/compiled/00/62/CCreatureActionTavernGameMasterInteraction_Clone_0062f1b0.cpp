struct CCreatureActionBase {};
struct CCreatureAction_TavernGameMasterInteraction : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_TavernGameMasterInteraction(const CCreatureAction_TavernGameMasterInteraction& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_TavernGameMasterInteraction::Clone() const {
    return new CCreatureAction_TavernGameMasterInteraction(*this);
}