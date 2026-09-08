struct CCreatureActionBase {};
struct CCreatureAction_BriarRoseShotFarOutof : CCreatureActionBase {
    unsigned char State[0x11c];
    CCreatureAction_BriarRoseShotFarOutof(const CCreatureAction_BriarRoseShotFarOutof& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_BriarRoseShotFarOutof::Clone() const { return new CCreatureAction_BriarRoseShotFarOutof(*this); }