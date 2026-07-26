struct CSystemManagerObserver
{
    virtual void Placeholder0() = 0;
    virtual void Placeholder1() = 0;
    virtual void Placeholder2() = 0;
    virtual void OnSystemUpdate() = 0;
};

enum ESystemReturn
{
    FABLE_SYSTEM_CONTINUE = 1,
    FABLE_SYSTEM_BUSY = 2
};

struct CSystemManager
{
    unsigned char unknown00[0x08];
    bool busy08;
    bool notifyObserver09;
    unsigned char unknown0A[0x72];
    CSystemManagerObserver* observer7C;
    unsigned char unknown80[0x6A];
    bool resetPendingEA;

    void UpdateActive();
    ESystemReturn Update();
};

ESystemReturn CSystemManager::Update()
{
    UpdateActive();
    if (!busy08)
    {
        if (resetPendingEA)
            resetPendingEA = false;
        if (notifyObserver09 && observer7C != 0)
            observer7C->OnSystemUpdate();
        return FABLE_SYSTEM_CONTINUE;
    }
    return FABLE_SYSTEM_BUSY;
}
