#include "engine/CInputProcessGameBase.h"

struct CInputProcessManager;

struct CInputProcessGameBase_Methods : CInputProcessGameBase {
    bool AddToInterface(CInputProcessManager* manager);
    bool Helper(CInputProcessManager* manager);
};

bool CInputProcessGameBase_Methods::AddToInterface(CInputProcessManager* manager)
{
    this->PGamePlayerInterface = (CGamePlayerInterface*)manager;
    return this->Helper(manager);
}
