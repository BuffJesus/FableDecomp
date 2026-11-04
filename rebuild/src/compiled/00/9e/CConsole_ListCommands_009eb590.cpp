#include "fable_generic_var.h"

// lib_console.cpp list commands (Ego_r static fastcall members, registered by
// CConsole::Initialise 0x009ED190). Each walks RegisteredConsoleInputs and sends
// matching names to the user; the "containing" forms match the lower-cased
// parameter anywhere in the lower-cased name. Retail 009EB590..009EB9B0.

typedef _STL::map<CCharString, CCountedPointer<CConsoleInputBase> >::iterator TInputsIterator;

// Retail 009EB590.
void FABLE_FASTCALL CConsole::ConsoleConsoleListContainingString(CConsole& owner, const CGenericVarVector& params)
{
    for (TInputsIterator it = owner.RegisteredConsoleInputs.begin(); it != owner.RegisteredConsoleInputs.end(); ++it)
    {
        if (it->second->GetName().GetAsLowerCase().Contains(params[0].GetString().GetAsLowerCase()))
            owner.SendMessageToUser(it->second->GetName());
    }
}

// Retail 009EB690.
void FABLE_FASTCALL CConsole::ConsoleCommandList(CConsole& owner, const CGenericVarVector&)
{
    for (TInputsIterator it = owner.RegisteredConsoleInputs.begin(); it != owner.RegisteredConsoleInputs.end(); ++it)
    {
        if (it->second->GetTypeAsString() == "command")
            owner.SendMessageToUser(it->second->GetName());
    }
}

// Retail 009EB760.
void FABLE_FASTCALL CConsole::ConsoleCommandListContainingString(CConsole& owner, const CGenericVarVector& params)
{
    for (TInputsIterator it = owner.RegisteredConsoleInputs.begin(); it != owner.RegisteredConsoleInputs.end(); ++it)
    {
        if (it->second->GetTypeAsString() == "command" &&
            it->second->GetName().GetAsLowerCase().Contains(params[0].GetString().GetAsLowerCase()))
            owner.SendMessageToUser(it->second->GetName());
    }
}

// Retail 009EB8E0.
void FABLE_FASTCALL CConsole::ConsoleVariableList(CConsole& owner, const CGenericVarVector&)
{
    for (TInputsIterator it = owner.RegisteredConsoleInputs.begin(); it != owner.RegisteredConsoleInputs.end(); ++it)
    {
        if (it->second->GetTypeAsString() == "variable")
            owner.SendMessageToUser(it->second->GetName());
    }
}

// Retail 009EB9B0.
void FABLE_FASTCALL CConsole::ConsoleVariableListContainingString(CConsole& owner, const CGenericVarVector& params)
{
    for (TInputsIterator it = owner.RegisteredConsoleInputs.begin(); it != owner.RegisteredConsoleInputs.end(); ++it)
    {
        if (it->second->GetTypeAsString() == "variable" &&
            it->second->GetName().GetAsLowerCase().Contains(params[0].GetString().GetAsLowerCase()))
            owner.SendMessageToUser(it->second->GetName());
    }
}
