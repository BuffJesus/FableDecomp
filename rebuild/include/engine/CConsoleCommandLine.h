#pragma once
// Ego_r member names, retail 0x10 layout; constructors inlined at 0x009ECD80.
#include "fable_base_class.h"
#include "fable_string.h"
class CConsoleCommandLine : public CBaseClass {
public:
    CConsoleCommandLine() : CurrentEntry(""), UserEntry(false), CursorPosition(0) {}
    // Not explicit: SetGuessMode 0x009EAFF0 assigns a CCharString to a line
    // through it (the call to 0x009EEA10 builds the temporary).
    CConsoleCommandLine(const CCharString& text) : CurrentEntry(text), UserEntry(false), CursorPosition(text.GetLength()) {}
    CConsoleCommandLine(const CConsoleCommandLine& other)
        : CBaseClass(), CurrentEntry(other.CurrentEntry), UserEntry(other.UserEntry), CursorPosition(other.CursorPosition) {}

private:
    friend class CConsole;
    // Ego_r/FableWin ?Reset@CConsoleCommandLine@@AAEXXZ (private); retail 0x009EA4F0.
    void Reset();
    // Ego_r private members, defined in lib_console2.cpp: the console's own
    // functions there expand them, and retail keeps out-of-line copies
    // (0x009EA4B0-0x009EA960, CConsoleCommandLine_Editing_009ea4b0.cpp). Other
    // units get the inline definitions below.
    // GetString: a copy of the entry (SetGuessMode 0x009EAFF0 copies it before
    // GetAsLowerCase).
    CCharString GetString() const;
    void SetAsUserEntry();
    bool IsUserEntry();
    void DecrementCursorPosition();
    long GetCursorPosition();
    void RemoveCharBeforeCursor();
    void RemoveCharAfterCursor();
    // SetString: the entry with the cursor at its end (CConsole::RunTextCommand
    // 0x009EC710 expands it; UserEntry is untouched).
    void SetString(const CCharString& input_string);
    void IncrementCursorPosition();
    // Retail 0x009EAB40.
    void AddCharAtCursor(long position, char ch);

public:
    CCharString CurrentEntry;
    bool UserEntry;
    long CursorPosition;
};
FABLE_STATIC_ASSERT(sizeof(CConsoleCommandLine) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CConsoleCommandLine, CurrentEntry) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CConsoleCommandLine, CursorPosition) == 0x0C);

#ifndef FABLE_CONSOLE_COMMAND_LINE_OUT_OF_LINE
inline CCharString CConsoleCommandLine::GetString() const { return CurrentEntry; }
inline void CConsoleCommandLine::SetAsUserEntry() { UserEntry = true; }
inline bool CConsoleCommandLine::IsUserEntry() { return UserEntry; }
inline void CConsoleCommandLine::DecrementCursorPosition()
{
    if (CursorPosition > 0)
        --CursorPosition;
}
inline long CConsoleCommandLine::GetCursorPosition() { return CursorPosition; }
inline void CConsoleCommandLine::RemoveCharBeforeCursor()
{
    if (CursorPosition > 0)
    {
        --CursorPosition;
        CurrentEntry.Erase(CursorPosition, 1);
    }
}
inline void CConsoleCommandLine::RemoveCharAfterCursor()
{
    if (CursorPosition < CurrentEntry.GetLength())
        CurrentEntry.Erase(CursorPosition, 1);
}
inline void CConsoleCommandLine::SetString(const CCharString& input_string)
{
    CurrentEntry = input_string;
    CursorPosition = input_string.GetLength();
}
inline void CConsoleCommandLine::IncrementCursorPosition()
{
    if (CursorPosition < CurrentEntry.GetLength())
        ++CursorPosition;
}
inline void CConsoleCommandLine::AddCharAtCursor(long position, char ch)
{
    CurrentEntry.Insert(position, ch);
    IncrementCursorPosition();
}
#endif
