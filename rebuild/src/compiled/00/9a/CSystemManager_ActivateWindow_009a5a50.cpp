// CSystemManager::ActivateWindow, retail 009A5A50. FableWin supplies the
// private static fastcall signature; sound slot identities are retail-only.
#include "fable_system_activation.h"

void FABLE_FASTCALL CSystemManager::ActivateWindow(CSystemManager* system, bool active)
{
    if (active && !system->ApplicationActive && system->EverBeenActive == true)
        system->Restore = true;
    system->ApplicationActive = active;
    system->EverBeenActive = true;
    if (active) {
        system->SetHasInputFocus(true);
        if (system->PInputManager.Data)
            system->PInputManager.Data->ClearEvents();
        if (system->PSoundManager.Data && FableSoundDeactivated_013ca610) {
            system->PSoundManager.Data->UnknownSlot46(0x8000, FableSavedSoundVolume_013ca60c, 0.0f);
            system->PSoundManager.Data->UnknownSetEnabled_009915e0(true);
            system->ForceMusicPlay = true;
            FableSoundDeactivated_013ca610 = false;
        }
    } else {
        system->SetHasInputFocus(false);
        if (system->PSoundManager.Data && !FableSoundDeactivated_013ca610) {
            FableSavedSoundVolume_013ca60c = system->PSoundManager.Data->UnknownSlot47(0x8000);
            system->PSoundManager.Data->UnknownSlot46(0x8000, 0.0f, 0.0f);
            if (!system->PSoundManager.Data->UnknownSlot49(system->ForceMusicPlayOffset))
                system->ForceMusicPlayOffset = 0.0f;
            system->PSoundManager.Data->UnknownSetEnabled_009915e0(false);
            FableSoundDeactivated_013ca610 = true;
        }
        if (system->PInputManager.Data)
            system->PInputManager.Data->ClearEvents();
    }
}

inline void CSystemManager::SetHasInputFocus(bool focus)
{
    ApplicationHasInputFocus = focus;
    if (PInputManager.Data)
        PInputManager.Data->SetHasInputFocus(focus);
}
