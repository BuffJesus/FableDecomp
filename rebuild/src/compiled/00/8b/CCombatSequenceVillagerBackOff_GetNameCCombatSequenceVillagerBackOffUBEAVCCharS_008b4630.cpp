// CCombatSequence_VillagerBackOff::GetName  retail 0x008b4630
// virtual CCharString GetName() const  -- returns literal name by value (NRVO).

struct CCharString {
    char* m_pData;
    // ctor(const char* str, int len) lives at 0x0099ebf0 in retail
    CCharString(const char* str, int len);
};

struct CActionDoCreatureAction {
    virtual CCharString GetName() const;
};

CCharString CActionDoCreatureAction::GetName() const
{
    return CCharString("CombatSequence_VillagerBackOff", -1);
}