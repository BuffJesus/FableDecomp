struct CCreatureActionBase {};
struct CCreatureAction_BlockRespond : CCreatureActionBase {
    unsigned char State[0x134];
    CCreatureAction_BlockRespond(const CCreatureAction_BlockRespond& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_BlockRespond::Clone() const { return new CCreatureAction_BlockRespond(*this); }