class CMainGameComponent
{
public:
    bool IsTimeForServerUpdate(long frame) const;
};

class CNetworkClient
{
public:
    bool IsFreeToRender() const;

private:
    unsigned char m_Pad00[0x2662];
    bool m_LocalGame;
    unsigned char m_Pad2663[0x09];
    long m_LocalFrame;
    unsigned char m_Pad2670[0x08];
    CMainGameComponent* m_GameComponent;
};

bool CNetworkClient::IsFreeToRender() const
{
    if (m_LocalGame)
        return !m_GameComponent->IsTimeForServerUpdate(m_LocalFrame);
    return true;
}
