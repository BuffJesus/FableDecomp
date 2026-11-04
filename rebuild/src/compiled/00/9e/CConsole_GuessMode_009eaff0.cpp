#include <string.h>
#include "fable_console.h"

// lib_console.cpp command completion (Ego_r private members; local
// `entry_so_far` from Ego_r.pdb). The guess walks RegisteredConsoleInputs from
// the lower bound of the lower-cased back buffer and offers each input whose
// name starts with it in the front buffer.

// Retail 009EAFF0.
void CConsole::SetGuessMode()
{
    GuessMode = true;
    CCharString entry_so_far = BackCommandLine.GetString().GetAsLowerCase();
    GuessPos = RegisteredConsoleInputs.lower_bound(entry_so_far);
    if (GuessPos != RegisteredConsoleInputs.end() && GuessPos->first.StartsWith(entry_so_far))
    {
        FrontCommandLine = GuessPos->second->GetFormattedName();
    }
}

// Retail 009EB0D0: nothing to cycle while the front buffer still shows just
// what was typed (compared case-insensitively: _stricmp, 0x00BFEBA8); otherwise offer the next input, wrapping round through
// SetGuessMode when the names stop matching.
void CConsole::NextCommandGuess()
{
    CCharString entry_so_far = BackCommandLine.GetString().GetAsLowerCase();
    if (FrontCommandLine.GetString().EqualsCaseInsensitive(entry_so_far))
        return;
    ++GuessPos;
    if (GuessPos != RegisteredConsoleInputs.end() && GuessPos->first.StartsWith(entry_so_far))
    {
        FrontCommandLine = GuessPos->second->GetFormattedName();
    }
    else
    {
        SetGuessMode();
    }
}
