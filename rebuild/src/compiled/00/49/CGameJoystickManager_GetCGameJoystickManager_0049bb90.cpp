// CGameJoystickManager::Get (retail 0x0049BB90).
// FableWin.pdb ?Get@CGameJoystickManager@@SIAAV1@XZ: create-on-first-use singleton
// held by the static counted pointer PClass.
#include "fable_input_event.h"

CGameJoystickManager& FABLE_FASTCALL CGameJoystickManager::Get()
{
    if (!PClass.Get())
        PClass.Reset(new CGameJoystickManager);
    return *PClass;
}