#include "fable_input_manager.h"
// Switch every device's exclusive mode; joystick slots may be empty.
bool CInputManager::SetExclusiveMode(bool exclusive)
{
    if (PMouse.get() != 0)
        PMouse->SetExclusiveMode(exclusive);
    if (PKeyboard.get() != 0)
        PKeyboard->SetExclusiveMode(exclusive);
    const long count = PJoysticks.size();
    for (long index = 0; index < count; ++index)
        if (PJoysticks[index].Get())
            PJoysticks[index]->SetExclusiveMode(exclusive);
    return true;
}
