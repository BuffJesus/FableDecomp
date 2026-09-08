struct CCreatureActionBase {};
struct CCreatureAction_CastAssassinRush : CCreatureActionBase {
    unsigned char State[0x138];
    CCreatureAction_CastAssassinRush(const CCreatureAction_CastAssassinRush& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_CastAssassinRush::Clone() const { return new CCreatureAction_CastAssassinRush(*this); }