struct CGameEvent
{
    long type;
    unsigned char player;
    unsigned char data[0x20];
    unsigned char endPos;
    unsigned char valid;
    unsigned char replacement;
    void Append(long size, const void* source);
};
struct CGameEventPackage
{
    unsigned long sequence;
    unsigned long eventCount;
    CGameEvent events[40];
    void AddEvent(CGameEvent* event);
    void Clear();
};
struct CGameEventPackageSet
{
    unsigned long packageCount;
    CGameEventPackage packages[50];
    void Initialise();
    void AddPackage(CGameEventPackage* package);
};
class CMainGameComponent
{
public:
    bool IsTimeForServerUpdate(long frame);
};
class CNetworkClient
{
public:
    unsigned char unused0000[0x2010];
    CGameEventPackage localEventPackage;
    unsigned char unused2658[0x14];
    long localFrame;
    unsigned long checksum1;
    unsigned long checksum2;
    CMainGameComponent* gameComponent;

    bool GetLocalGameEventPackageSet(CGameEventPackageSet* packageSet);
};

bool CNetworkClient::GetLocalGameEventPackageSet(CGameEventPackageSet* packageSet)
{
    CGameEvent event;
    unsigned long copySource;
    event.player = 0xFF;
    event.valid = 0;
    unsigned char updateReady = static_cast<unsigned char>(
        gameComponent->IsTimeForServerUpdate(localFrame));
    if (updateReady == 1)
    {
        copySource = checksum1;
        event.type = 1;
        event.endPos = 0;
        event.valid = updateReady;
        event.replacement = 0;
        event.player = 0xFF;
        event.Append(4, &copySource);
        copySource = checksum2;
        event.Append(4, &copySource);
        copySource = static_cast<unsigned long>(localFrame);
        event.Append(4, &copySource);
        ++localFrame;
        localEventPackage.AddEvent(&event);
        localEventPackage.sequence = static_cast<unsigned long>(localFrame);
        packageSet->Initialise();
        packageSet->AddPackage(&localEventPackage);
        localEventPackage.Clear();
        return true;
    }
    return false;
}