
#include <cstdio>

class CInputProcessManager;

class CInputProcessGameBase
{
public:
    char pad[0x24];
    int m_field28;
    virtual bool AddToInterface(CInputProcessManager &mgr);
};

bool CInputProcessGameBase::AddToInterface(CInputProcessManager &mgr)
{
    printf("BASE_CALLED field28=%d\n", m_field28);
    return true;
}

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

int main()
{
    CInputProcessWatchForRangedWeaponThirdPersonModeTermination obj;
    obj.m_field28 = 0;
    CInputProcessManager *mgrPtr = 0;
    bool r = obj.AddToInterface(*mgrPtr);
    if (r && obj.m_field28 == -1) {
        printf("RESULT_PASS_MARKER\n");
    } else {
        printf("RESULT_FAIL field28=%d r=%d\n", obj.m_field28, (int)r);
    }
    return 0;
}