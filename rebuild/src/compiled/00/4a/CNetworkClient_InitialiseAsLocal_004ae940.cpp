class CMainGameComponent;

class CNetworkClient
{
public:
    unsigned char unused00[0x2660];
    bool initialised;                 // +0x2660
    bool host;                        // +0x2661
    bool updateEnabled;               // +0x2662
    unsigned char unused2663;
    void* localPlayer;                // +0x2664
    void* hostPlayer;                 // +0x2668
    unsigned long localFrame;         // +0x266C
    unsigned char unused2670[8];
    CMainGameComponent* gameComponent; // +0x2678

    bool InitialiseNetworkBase();
    bool InitialiseAsLocal(CMainGameComponent* component);
};

bool CNetworkClient::InitialiseAsLocal(CMainGameComponent* component)
{
    if (InitialiseNetworkBase() == true)
    {
        host = false;
        localPlayer = 0;
        hostPlayer = 0;
        localFrame = 0;
        gameComponent = component;
        updateEnabled = true;
        initialised = true;
        return true;
    }
    return false;
}