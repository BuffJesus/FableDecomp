class CInputProcessManager;

class CInputProcessWatchForBaseThirdPersonModeTermination
{
public:
    int pad[12];
    bool AddToInterface(CInputProcessManager& mgr);
};

class CInputProcessWatchForWillChargeUpThirdPersonModeTermination : public CInputProcessWatchForBaseThirdPersonModeTermination
{
public:
    bool AddToInterface(CInputProcessManager& mgr);
};

bool CInputProcessWatchForWillChargeUpThirdPersonModeTermination::AddToInterface(CInputProcessManager& mgr)
{
    *(int*)((char*)this + 0x28) = -1;
    return CInputProcessWatchForBaseThirdPersonModeTermination::AddToInterface(mgr);
}