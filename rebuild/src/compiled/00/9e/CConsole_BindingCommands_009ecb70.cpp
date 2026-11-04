#include "fable_generic_var.h"

// lib_console.cpp script and binding command bindings (Ego_r static fastcall
// members registered by CConsole::Initialise 0x009ED190). The private
// Add/Run helpers are expanded into them. Retail 009ECB70..009ECD40; the
// string-binding pair is absent from the manifest.

void CConsole::AddKeyBinding(char c, const CCharString& text)
{
    KeyBindingMap[c] = text;
}

void CConsole::AddStringBinding(const CCharString& str, const CCharString& text)
{
    StringBindingMap[str] = text;
}

void CConsole::RunStringBinding(const CCharString& str)
{
    _STL::map<CCharString, CCharString>::iterator it = StringBindingMap.find(str);
    if (it != StringBindingMap.end())
        RunTextCommand(it->second);
}

// Retail 009ECB70: scripts default to the ".ini" extension (Ego_r local script_name).
void FABLE_FASTCALL CConsole::ConsoleRunScript(CConsole& owner, const CGenericVarVector& params)
{
    CCharString script_name = params[0].GetString();
    if (!script_name.GetAsLowerCase().EndsWith(".ini"))
        script_name += ".ini";
    owner.RunScript(script_name.ToWideString());
}

// Retail 009ECC70.
void FABLE_FASTCALL CConsole::ConsoleAddKeyBinding(CConsole& owner, const CGenericVarVector& params)
{
    owner.AddKeyBinding(params[0].GetString().GetConstCharPtr()[0], params[1].GetString());
}

// Retail 009ECCE0.
void FABLE_FASTCALL CConsole::ConsoleAddStringBinding(CConsole& owner, const CGenericVarVector& params)
{
    owner.AddStringBinding(params[0].GetString(), params[1].GetString());
}

// Retail 009ECD40.
void FABLE_FASTCALL CConsole::ConsoleRunStringBinding(CConsole& owner, const CGenericVarVector& params)
{
    owner.RunStringBinding(params[0].GetString());
}
