#include "engine/CInputProcessGameBase.h"
#include <cstdio>

static void* g_manager = 0;
struct CInputProcessManager;

struct CInputProcessGameBase_Methods : CInputProcessGameBase {
    bool AddToInterface(CInputProcessManager* manager);
    bool Helper(CInputProcessManager* manager);
};

bool CInputProcessGameBase_Methods::Helper(CInputProcessManager* manager)
{
    g_manager = manager;
    return true;
}

bool CInputProcessGameBase_Methods::AddToInterface(CInputProcessManager* manager)
{
    this->PGamePlayerInterface = (CGamePlayerInterface*)manager;
    return this->Helper(manager);
}

int main()
{
    int value;
    CInputProcessGameBase_Methods object;
    bool result = object.AddToInterface((CInputProcessManager*)&value);
    if (!result || (void*)object.PGamePlayerInterface != &value || g_manager != &value) {
        std::printf("00687a70_TEST FAIL\n");
        return 1;
    }
    std::printf("00687a70_TEST PASS\n");
    return 0;
}
