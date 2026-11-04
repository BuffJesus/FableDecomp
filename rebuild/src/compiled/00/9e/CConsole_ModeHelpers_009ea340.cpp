// lib_console2.cpp, retail 0x009EA340-0x009EAA40: out-of-line copies of the
// console's private mode helpers (Ego_r names), and the parameter list's typed
// placeholder.
#define FABLE_CONSOLE_MODE_OUT_OF_LINE
#include "fable_console.h"
#include "fable_generic_var.h"

// Retail 009EA340.
bool CConsole::IsHistoryMode() const
{
    return HistoryMode;
}

// Retail 009EA350.
bool CConsole::IsGuessMode() const
{
    return GuessMode;
}

// Retail 009EA560.
CCharString CConsole::GetHelpBarString() const
{
    return HelpBarString;
}

// Retail 009EA580.
void CConsole::UnsetGuessMode()
{
    GuessMode = false;
}

// Retail 009EA590.
void CConsole::UnsetHistoryMode()
{
    HistoryMode = false;
}

// Retail 009EA600: accept the guess or history line on screen.
void CConsole::ConfirmAlternative()
{
    if (IsGuessMode())
        UnsetGuessMode();
    else if (IsHistoryMode())
        UnsetHistoryMode();
}

// Retail 009EA6C0.
CGenericVar CConsoleCommandParameters::GetCorrectlyTypedParameter(long index) const
{
    CGenericVar var;
    var.Type = ParamTypes[index];
    return var;
}

// Retail 009EAA40.
void CConsole::ResetHistory()
{
    HistoryPos = CommandLineHistory.begin();
}
