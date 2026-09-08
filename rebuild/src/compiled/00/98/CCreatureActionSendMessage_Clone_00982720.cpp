struct CCreatureActionBase {};
struct CCreatureAction_SendMessage : CCreatureActionBase {
    unsigned char State[0xc4];
    CCreatureAction_SendMessage(const CCreatureAction_SendMessage& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_SendMessage::Clone() const { return new CCreatureAction_SendMessage(*this); }