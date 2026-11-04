#include "fable_system_manager_state.h"
#include "fable_sound_manager.h"
ESystemReturn CSystemManager::Update()
{
    UpdateActive();
    if (!WindowsQuit) {
        if (Restore) Restore=false;
        if (ApplicationActive && PSoundManager.Data)
            PSoundManager->UnknownSlot03();
        return ESR_OK;
    }
    return ESR_QUIT;
}
