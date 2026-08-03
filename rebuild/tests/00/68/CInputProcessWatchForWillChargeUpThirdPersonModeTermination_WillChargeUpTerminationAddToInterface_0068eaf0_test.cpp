#include <cstdio>

class CInputProcessManager;

class CInputProcessWatchForBaseThirdPersonModeTermination
{
public:
    int pad[12];
    bool AddToInterface(CInputProcessManager& mgr);
};

bool CInputProcessWatchForBaseThirdPersonModeTermination::AddToInterface(CInputProcessManager& mgr)
{
    printf("BASE_CALLED this28=%d\n", *(int*)((char*)this + 0x28));
    return true;
}

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

int main()
{
    CInputProcessWatchForWillChargeUpThirdPersonModeTermination obj;
    for (int i = 0; i < 12; i++) ((int*)&obj)[i] = 0;
    CInputProcessManager* mgrp = (CInputProcessManager*)0x1234;
    bool r = obj.AddToInterface(*mgrp);
    if (r && *(int*)((char*)&obj + 0x28) == -1)
        puts("PASS_0068eaf0");
    else
        puts("FAIL_0068eaf0");
    return 0;
}