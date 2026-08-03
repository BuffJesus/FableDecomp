class CCombatActionBase
{
public:
    char pad[0xB8];
    unsigned long m_meleeFlourish;

    bool IsMeleeFlourish();
};

bool CCombatActionBase::IsMeleeFlourish()
{
    if (m_meleeFlourish)
        return true;
    return false;
}