struct CCreatureActionBase {};
struct CCreatureAction_BlowHorn : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_BlowHorn(const CCreatureAction_BlowHorn& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_BlowHorn::Clone() const { return new CCreatureAction_BlowHorn(*this); }