#include "fable_console.h"
CConsole::CConsole()
    : VisibleHistoryStart(CommandLineHistory.begin()),
      VisibleHistoryFinish(CommandLineHistory.begin()),
      NumberOfHistoryLinesOnscreen(9), HistoryMode(false), MovingIntoHistory(true),
      GuessMode(false), GuessPos(0), PFont(0), Active(false), ConsoleAlpha(255),
      HistoryWindowColour(60,60,60), ClientWindowColour(0,0,0),
      HelpWindowColour(60,60,60), ScrollBarColour(0,0,0), TextColour(255,255,255),
      CursorBlink(0), CursorBlinkRate(32)
{
}
