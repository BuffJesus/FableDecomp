#include "fable_ime.h"
#include "fable_gfuninitialise.h"
#include "fable_surface.h"

void NUISystem::CIME::Enable(bool enable)
{
    if (IsInitialised)
    {
        ImeUi_FinalizeString(false);
        ImeUi_EnableIme(enable);
    }
}

void NUISystem::CIME::SetScreenDimensions(long width, long height)
{
    if (IsInitialised)
        ImeUi_SetScreenDimension(width, height);
}

void NUISystem::CIME::Initialise(HWND__* window)
{
    Shutdown();
    if (EnableIME)
    {
        ImeUiCallback_DrawRect = Callback_DrawRect;
        ImeUiCallback_Malloc = Callback_malloc;
        ImeUiCallback_Free = Callback_free;
        ImeUiCallback_DrawFans = Callback_DrawFans;
        ImeUiCallback_OnChar = Callback_OnChar;
        WindowHandle = window;
        IsInitialised = ImeUi_Initialize(window, false);
        if (IsInitialised)
        {
            Appearance.symbolFont = new CIMEFont;
            ImeUi_SetAppearance(&Appearance);
            CSystemManager* system = GFGetSystemManager();
            const C2DExtentsI dimensions =
                system->PDisplayManager.Data->GetRenderTargetDimensions();
            SetScreenDimensions(dimensions.X, dimensions.Y);
        }
        Enable(false);
    }
    else
    {
        ImeUi_Initialize(window, true);
    }
}
