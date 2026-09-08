struct CCreatureActionBase {};
struct CCreatureAction_UseObject : CCreatureActionBase {
    unsigned char State[0xe0];
    CCreatureAction_UseObject(const CCreatureAction_UseObject& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_UseObject::Clone() const {
    return new CCreatureAction_UseObject(*this);
}