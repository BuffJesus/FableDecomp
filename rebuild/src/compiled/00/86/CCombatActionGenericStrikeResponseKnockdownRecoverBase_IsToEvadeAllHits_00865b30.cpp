class CCombatAction_GenericStrikeResponseKnockdownRecoverBase
{
public:
    char pad_0000[0x114];
    unsigned char m_ToEvadeAllHits;

    unsigned char __fastcall IsToEvadeAllHits(void*);
};

unsigned char __fastcall CCombatAction_GenericStrikeResponseKnockdownRecoverBase::IsToEvadeAllHits(void*)
{
    return m_ToEvadeAllHits == 0;
}