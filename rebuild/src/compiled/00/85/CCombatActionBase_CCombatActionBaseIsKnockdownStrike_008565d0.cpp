class CCombatActionBase
{
public:
    char pad[0xB8];
    unsigned long m_knockdownFlag;

    bool __fastcall IsKnockdownStrike(void* /*edx*/);
};

bool __fastcall CCombatActionBase::IsKnockdownStrike(void* /*edx*/)
{
    if (m_knockdownFlag)
        return true;
    return false;
}