struct CCreatureActionBase {};
struct CCreatureAction_HobbeSpellcasterAim : CCreatureActionBase {
    unsigned char State[0x11c];
    CCreatureAction_HobbeSpellcasterAim(const CCreatureAction_HobbeSpellcasterAim& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_HobbeSpellcasterAim::Clone() const { return new CCreatureAction_HobbeSpellcasterAim(*this); }