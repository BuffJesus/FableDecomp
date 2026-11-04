// lib_console2.cpp, retail 0x009EABE0-0x009EAD10: moving the visible window
// over the command-line history (Ego_r private CConsole members). The window
// only moves while the history is longer than the lines on screen.
#include "fable_console.h"

// Retail 009EABE0.
void CConsole::ScrollUpThroughHistory()
{
    if (CommandLineHistory.size() > NumberOfHistoryLinesOnscreen)
    {
        for (long i = 0; i < NumberOfHistoryLinesOnscreen; ++i)
        {
            if (VisibleHistoryFinish == CommandLineHistory.end())
                return;
            ++VisibleHistoryStart;
            ++VisibleHistoryFinish;
        }
    }
}

// Retail 009EAC30.
void CConsole::ScrollDownThroughHistory()
{
    if (CommandLineHistory.size() > NumberOfHistoryLinesOnscreen)
    {
        for (long i = 0; i < NumberOfHistoryLinesOnscreen; ++i)
        {
            if (VisibleHistoryStart == CommandLineHistory.begin())
                return;
            --VisibleHistoryStart;
            --VisibleHistoryFinish;
        }
    }
}

// Retail 009EAD10. Near match: retail pops esi straight after the size test,
// VC7.1 here at each exit (4 lines; early-return, chained-assignment and
// separate-count forms all compile the same).
void CConsole::ScrollToHistoryEnd()
{
    if (CommandLineHistory.size() > NumberOfHistoryLinesOnscreen)
    {
        VisibleHistoryFinish = CommandLineHistory.end();
        VisibleHistoryStart = CommandLineHistory.end();
        _STL::advance(VisibleHistoryStart, -NumberOfHistoryLinesOnscreen);
    }
}
