#include "fable_input_manager.h"
void CInputManager::SetHasInputFocus(bool hasFocus)
{
    if (PMouse.get() != 0)
        PMouse->SetHasInputFocus(hasFocus);
    if (PKeyboard.get() != 0)
        PKeyboard->SetHasInputFocus(hasFocus);
    const long count = PJoysticks.size();
    for (long index = 0; index < count; ++index)
        PJoysticks[index]->SetHasInputFocus(hasFocus);
}
