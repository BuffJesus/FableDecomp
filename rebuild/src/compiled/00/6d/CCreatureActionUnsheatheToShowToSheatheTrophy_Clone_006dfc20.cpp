struct CCreatureActionBase {};
struct CCreatureAction_UnsheatheToShowToSheatheTrophy : CCreatureActionBase {
    unsigned char State[0xc4];
    CCreatureAction_UnsheatheToShowToSheatheTrophy(const CCreatureAction_UnsheatheToShowToSheatheTrophy& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_UnsheatheToShowToSheatheTrophy::Clone() const { return new CCreatureAction_UnsheatheToShowToSheatheTrophy(*this); }