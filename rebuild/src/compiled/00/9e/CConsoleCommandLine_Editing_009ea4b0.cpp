// lib_console2.cpp, retail 0x009EA4B0-0x009EAB40: the out-of-line copies of
// CConsoleCommandLine's private editing members (Ego_r names; the console's
// own functions in that file expand them).
#define FABLE_CONSOLE_COMMAND_LINE_OUT_OF_LINE
#include "engine/CConsoleCommandLine.h"

// Retail 009EA4B0.
CCharString CConsoleCommandLine::GetString() const
{
    return CurrentEntry;
}

// Retail 009EA4D0.
void CConsoleCommandLine::SetAsUserEntry()
{
    UserEntry = true;
}

// Retail 009EA4E0.
bool CConsoleCommandLine::IsUserEntry()
{
    return UserEntry;
}

// Retail 009EA510.
void CConsoleCommandLine::DecrementCursorPosition()
{
    if (CursorPosition > 0)
        --CursorPosition;
}

// Retail 009EA520.
long CConsoleCommandLine::GetCursorPosition()
{
    return CursorPosition;
}

// Retail 009EA5E0.
void CConsoleCommandLine::RemoveCharBeforeCursor()
{
    if (CursorPosition > 0)
    {
        --CursorPosition;
        CurrentEntry.Erase(CursorPosition, 1);
    }
}

// Retail 009EA910.
void CConsoleCommandLine::RemoveCharAfterCursor()
{
    if (CursorPosition < CurrentEntry.GetLength())
        CurrentEntry.Erase(CursorPosition, 1);
}

// Retail 009EA930.
void CConsoleCommandLine::SetString(const CCharString& input_string)
{
    CurrentEntry = input_string;
    CursorPosition = input_string.GetLength();
}

// Retail 009EA960.
void CConsoleCommandLine::IncrementCursorPosition()
{
    if (CursorPosition < CurrentEntry.GetLength())
        ++CursorPosition;
}

// Retail 009EAB40.
void CConsoleCommandLine::AddCharAtCursor(long position, char ch)
{
    CurrentEntry.Insert(position, ch);
    IncrementCursorPosition();
}
