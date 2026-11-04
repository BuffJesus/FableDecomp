#include "fable_system_manager.h"

void CSystemManager::SetHasInputFocus(bool hasFocus)
{
    ApplicationHasInputFocus = hasFocus;
    if (PInputManager.Get())
        PInputManager->SetHasInputFocus(hasFocus);
}
