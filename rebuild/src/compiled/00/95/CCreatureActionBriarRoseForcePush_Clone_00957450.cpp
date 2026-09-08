struct CCreatureActionBase {};
struct CCreatureAction_BriarRoseForcePush : CCreatureActionBase {
    unsigned char State[0x11c];
    CCreatureAction_BriarRoseForcePush(const CCreatureAction_BriarRoseForcePush& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_BriarRoseForcePush::Clone() const { return new CCreatureAction_BriarRoseForcePush(*this); }