#include <cstdio>
#include <cstring>

class CMainGameComponent
{
public:
    bool IsTimeForServerUpdate(long frame) const;
};

class CNetworkClient
{
public:
    bool IsFreeToRender() const;

    void SetState(
        bool localGame,
        long localFrame,
        CMainGameComponent* gameComponent)
    {
        m_LocalGame = localGame;
        m_LocalFrame = localFrame;
        m_GameComponent = gameComponent;
    }

private:
    unsigned char m_Pad00[0x2662];
    bool m_LocalGame;
    unsigned char m_Pad2663[0x09];
    long m_LocalFrame;
    unsigned char m_Pad2670[0x08];
    CMainGameComponent* m_GameComponent;
};

static const CMainGameComponent* g_ObservedComponent;
static long g_ObservedFrame;
static unsigned long g_CallCount;
static bool g_TimeForServerUpdate;

bool CMainGameComponent::IsTimeForServerUpdate(long frame) const
{
    g_ObservedComponent = this;
    g_ObservedFrame = frame;
    ++g_CallCount;
    return g_TimeForServerUpdate;
}

int main()
{
    CMainGameComponent component;
    CNetworkClient client;
    std::memset(&client, 0, sizeof(client));

    client.SetState(false, 0x12345678, 0);
    if (!client.IsFreeToRender() || g_CallCount != 0)
        return 1;

    client.SetState(true, -1234567, &component);
    g_TimeForServerUpdate = false;
    if (!client.IsFreeToRender())
        return 2;
    if (g_CallCount != 1 || g_ObservedComponent != &component ||
        g_ObservedFrame != -1234567)
        return 3;

    client.SetState(true, 0x76543210, &component);
    g_TimeForServerUpdate = true;
    if (client.IsFreeToRender())
        return 4;
    if (g_CallCount != 2 || g_ObservedComponent != &component ||
        g_ObservedFrame != 0x76543210)
        return 5;

    std::printf("AUTO_004aea70_TEST PASS\n");
    return 0;
}
