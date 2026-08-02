
class CInputProcessManager;

class CInputProcessGameBase
{
public:
    char pad[0x24];
    int m_field28;
    virtual bool AddToInterface(CInputProcessManager &mgr);
};

class CInputProcessWatchForRangedWeaponThirdPersonModeTermination : public CInputProcessGameBase
{
public:
    virtual bool AddToInterface(CInputProcessManager &mgr);
};

bool CInputProcessWatchForRangedWeaponThirdPersonModeTermination::AddToInterface(CInputProcessManager &mgr)
{
    m_field28 = -1;
    return CInputProcessGameBase::AddToInterface(mgr);
}