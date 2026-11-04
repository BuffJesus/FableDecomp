#include "fable_console.h"
#include "fable_string_parser.h"

// One retail TU region (0x009EA4F0..0x009EA6C0): Reset and the buffer copies are
// emitted out of line and auto-inlined into the callers below. Reset 009EA4F0 and the
// copies 009EA620/009EA640 also have older stand-alone landings; this file is the
// genuine shared-header form of all five.

void CConsoleCommandLine::Reset()
{
    CurrentEntry = "";
    UserEntry = false;
    CursorPosition = 0;
}

void CConsole::ResetCommandLineBuffers()
{
    FrontCommandLine.Reset();
    BackCommandLine.Reset();
    UnsetGuessMode();
    UnsetHistoryMode();
}

void CConsole::CopyFrontBufferToBackBuffer()
{
    BackCommandLine = FrontCommandLine;
}

void CConsole::CopyBackBufferToFrontBuffer()
{
    FrontCommandLine = BackCommandLine;
}

// Leaving guess or history mode restores what the user had typed.
void CConsole::RevertToUserEntry()
{
    if (IsGuessMode()) {
        CopyBackBufferToFrontBuffer();
        UnsetGuessMode();
    } else if (IsHistoryMode()) {
        CopyBackBufferToFrontBuffer();
        UnsetHistoryMode();
    } else {
        FrontCommandLine.Reset();
    }
}

// Retail 009EA980: step back to the previous user-typed history line, if any.
void CConsole::PrevInHistory()
{
    _STL::list<CConsoleCommandLine>::iterator saved = HistoryPos;
    bool found = false;
    while (HistoryPos != CommandLineHistory.end() && !found) {
        ++HistoryPos;
        if (HistoryPos != CommandLineHistory.end() && HistoryPos->UserEntry) {
            FrontCommandLine = *HistoryPos;
            found = true;
        }
    }
    if (!found)
        HistoryPos = saved;
}

// Retail 009EA9E0: step forward to the next user-typed history line, or back to
// what the user was typing.
void CConsole::NextInHistory()
{
    bool found = false;
    while (HistoryPos != CommandLineHistory.begin() && !found) {
        --HistoryPos;
        if (HistoryPos->UserEntry) {
            FrontCommandLine = *HistoryPos;
            found = true;
        }
    }
    if (!found)
        RevertToUserEntry();
}

// Retail 009EAB80: enter history mode on the most recent user-typed line.
void CConsole::SetHistoryMode()
{
    for (HistoryPos = CommandLineHistory.begin(); HistoryPos != CommandLineHistory.end(); ++HistoryPos) {
        if (HistoryPos->UserEntry) {
            FrontCommandLine = *HistoryPos;
            HistoryMode = true;
            return;
        }
    }
}

// Retail 009EC710 (Ego_r locals parser, parsed_item, command_line): run each
// ";"-separated command through the front line; a ";" inside quotes is kept.
void CConsole::RunTextCommand(const CCharString& text_cmd)
{
    CStringParser parser(text_cmd, 0, CCharString(""), 0);
    CParsedItem parsed_item;
    CCharString command_line;
    while (parser.ReadAsStringUntilString(";", parsed_item))
    {
        command_line += parsed_item.GetString();
        if (!parser.InQuotes)
        {
            FrontCommandLine.SetString(command_line);
            RunCommandLine();
            FrontCommandLine.Reset();
            command_line = "";
        }
        else
        {
            command_line += ";";
        }
        parser.ReadNextItem(parsed_item);
    }
}

// Retail 009ECA30: Enter. A non-empty line has its cursor run to the end, is
// marked as typed by the user, goes into the history and runs; the buffers
// are then cleared either way.
void CConsole::UserInputComplete()
{
    if (FrontCommandLine.GetString().GetLength() > 0)
    {
        while (FrontCommandLine.GetCursorPosition() < FrontCommandLine.GetString().GetLength())
            FrontCommandLine.IncrementCursorPosition();
        FrontCommandLine.SetAsUserEntry();
        AddCommandLineToHistory(FrontCommandLine);
        RunCommandLine();
    }
    ResetCommandLineBuffers();
}
