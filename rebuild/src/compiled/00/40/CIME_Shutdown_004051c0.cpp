#include <string.h>
#include "fable_ime.h"

void NUISystem::CIME::Shutdown()
{
    if (IsInitialised)
    {
        ImeUi_FinalizeString(false);
        ImeUi_EnableIme(false);
        ImeUi_Initialize(WindowHandle, true);
        IsInitialised = false;
        delete Appearance.symbolFont;
        Appearance.symbolFont = 0;
        EnableIME = false;
        IsInitialised = false;
        WindowHandle = 0;
        PEditBox = 0;
        memset(&Appearance, 0, sizeof(Appearance));
        ResetToDefaultSettings();
    }
}
