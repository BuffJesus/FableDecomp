#include "fable_ime.h"

extern NUISystem::CIME g_FableIME_013B7CD8;
long FABLE_FASTCALL ImeUi_ProcessMessage(
    HWND__*, unsigned int, unsigned int, long, bool*);
bool FABLE_FASTCALL ImeUi_IgnoreHotKey(const tagMSG*);
void FABLE_FASTCALL ImeUi_ToggleLanguageBar(int);

bool FABLE_FASTCALL NUISystem::CIME::ProcessWinMessage(
    HWND__* window, unsigned int message, unsigned int wParam,
    long lParam, long& result)
{
    if (g_FableIME_013B7CD8.IsInitialised)
    {
        bool trapped = false;
        long value = ImeUi_ProcessMessage(window, message, wParam, lParam, &trapped);
        if (trapped)
        {
            result = value;
            return true;
        }
        if (message == 0x1c)
        {
            ImeUi_ToggleLanguageBar(static_cast<unsigned short>(wParam) == 0);
            result = 0;
            return true;
        }
    }
    return false;
}

bool FABLE_FASTCALL NUISystem::CIME::FilterMessage(tagMSG* message)
{
    return ImeUi_IgnoreHotKey(message);
}
