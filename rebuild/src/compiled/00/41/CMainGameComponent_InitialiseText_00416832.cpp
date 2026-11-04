// CMainGameComponent::InitialiseText @ 0x00416832
// FableWin.pdb: ?InitialiseText@CMainGameComponent@@IAEXXZ.
// Create the main text bank and open the current language's bank file.
// MainGameComponent.cpp is size-optimised in retail and built without EH.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_game_text_bank.h"
#include "fable_language.h"
#include "fable_progress_display.h"

void CMainGameComponent::InitialiseText()
{
    NProgressDisplay::DisplayProgress("Init Text", -1.0f, false, false);
    PTextBank.Reset(new NGameText::CDataBank);
    NProgressDisplay::DisplayProgress("Opening Text Bank", -1.0f, false, false);
    // Retail 0x00415260 selects the current language's TEXT_<LANGUAGE>_MAIN.
    PTextBank.Get()->OpenReadOnly(NLocalisation::GetTextBankName(), 0x100);
}
