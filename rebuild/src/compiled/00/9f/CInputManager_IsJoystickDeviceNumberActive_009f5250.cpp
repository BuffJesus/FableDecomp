#include "fable_input_manager.h"
bool CInputManager::IsJoystickDeviceNumberActive(long deviceNumber) const
{
    const long count = PJoysticks.size();
    for (long index = 0; index < count; ++index)
        if (deviceNumber == PJoysticks[index]->GetJoystickDeviceNumber())
            return true;
    return false;
}
