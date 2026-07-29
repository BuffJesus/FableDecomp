#include <cstdio>
#include <cstring>

class CGameEventPackageSet
{
public:
    explicit CGameEventPackageSet(unsigned long identity)
        : m_Identity(identity)
    {
    }

    unsigned long m_Identity;
};

class CNetworkClient
{
public:
    bool GetGameEventPackageSet(CGameEventPackageSet* packageSet);
    void SetLocalGame(bool localGame)
    {
        m_LocalGame = localGame;
    }

private:
    bool GetLocalGameEventPackageSet(
        CGameEventPackageSet* packageSet);

    unsigned char m_Pad00[0x2662];
    bool m_LocalGame;
};

static CNetworkClient* g_ObservedClient;
static CGameEventPackageSet* g_ObservedPackageSet;
static unsigned long g_CallCount;
static bool g_Result;

bool CNetworkClient::GetLocalGameEventPackageSet(
    CGameEventPackageSet* packageSet)
{
    g_ObservedClient = this;
    g_ObservedPackageSet = packageSet;
    ++g_CallCount;
    return g_Result;
}

int main()
{
    CNetworkClient client;
    CGameEventPackageSet firstPackage(0x11223344);
    CGameEventPackageSet secondPackage(0x55667788);
    std::memset(&client, 0, sizeof(client));

    g_Result = true;
    client.SetLocalGame(false);
    if (client.GetGameEventPackageSet(&firstPackage))
        return 1;
    if (g_CallCount != 0)
        return 2;

    client.SetLocalGame(true);
    g_Result = false;
    if (client.GetGameEventPackageSet(&firstPackage))
        return 3;
    if (g_CallCount != 1 || g_ObservedClient != &client ||
        g_ObservedPackageSet != &firstPackage)
        return 4;

    g_Result = true;
    if (!client.GetGameEventPackageSet(&secondPackage))
        return 5;
    if (g_CallCount != 2 || g_ObservedClient != &client ||
        g_ObservedPackageSet != &secondPackage)
        return 6;

    std::printf("BATCH18_004aeba0_TEST PASS\n");
    return 0;
}
