// Text 009c7e00 and group 009c7eb0: unload only after the pending read finishes.
#include "fable_game_text_bank.h"
bool NGameText::CTextEntry::UnloadResource()
{
    if (IsLoaded()) Owner->PText.Reset();
    return true;
}
bool NGameText::CGroupEntry::UnloadResource()
{
    if (IsLoaded()) Owner->PGroup.Reset();
    return true;
}
