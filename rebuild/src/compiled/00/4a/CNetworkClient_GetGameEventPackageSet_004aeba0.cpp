class CGameEventPackageSet;

class CNetworkClient
{
public:
    bool GetGameEventPackageSet(CGameEventPackageSet* packageSet);

private:
    bool GetLocalGameEventPackageSet(
        CGameEventPackageSet* packageSet);

    unsigned char m_Pad00[0x2662];
    bool m_LocalGame;
};

bool CNetworkClient::GetGameEventPackageSet(
    CGameEventPackageSet* packageSet)
{
    if (m_LocalGame)
        return GetLocalGameEventPackageSet(packageSet);
    return false;
}
