struct CCreatureActionBase {};
struct CCreatureAction_Talk : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_Talk(const CCreatureAction_Talk& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_Talk::Clone() const {
    return new CCreatureAction_Talk(*this);
}