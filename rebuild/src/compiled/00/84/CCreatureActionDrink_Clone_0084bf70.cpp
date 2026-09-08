struct CCreatureActionBase {};
struct CCreatureAction_Drink : CCreatureActionBase {
    unsigned char State[0xb8];
    CCreatureAction_Drink(const CCreatureAction_Drink& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_Drink::Clone() const { return new CCreatureAction_Drink(*this); }