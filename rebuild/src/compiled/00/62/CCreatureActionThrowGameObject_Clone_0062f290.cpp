struct CCreatureActionBase {};
struct CCreatureAction_ThrowGameObject : CCreatureActionBase {
    unsigned char State[0xb4];
    CCreatureAction_ThrowGameObject(const CCreatureAction_ThrowGameObject& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_ThrowGameObject::Clone() const {
    return new CCreatureAction_ThrowGameObject(*this);
}