#include "fable_system_manager_state.h"

void CSystemManager::SetAsActive(bool active)
{
    if (active && !ApplicationActive && EverBeenActive == true)
        Restore = true;
    ApplicationActive = active;
    EverBeenActive = true;
}
