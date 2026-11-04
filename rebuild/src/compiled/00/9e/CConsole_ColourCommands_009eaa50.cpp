// lib_console.cpp, retail 0x009EAA50-0x009EAB3F: the console colour commands
// (Ego_r static fastcall members registered by CConsole::Initialise 0x009ED190
// with three ULONG parameters): red, green and blue, alpha opaque. Near
// matches, not gated: retail stores the four bytes straight into the member,
// loading the parameters right to left as dwords, where this form builds a
// temporary colour. A field-wise assignment gets closer, but none of the field
// orders tried reproduces retail's store order.
#include "fable_generic_var.h"
#include "fable_rgb_colour.h"

// Retail 009EAA50.
void FABLE_FASTCALL CConsole::SetConsoleTextColour(CConsole& owner, const CGenericVarVector& params)
{
    owner.TextColour = CRGBColour((unsigned char)params[0].GetULONG(), (unsigned char)params[1].GetULONG(),
                          (unsigned char)params[2].GetULONG(), 0xFF);
}

// Retail 009EAA80.
void FABLE_FASTCALL CConsole::SetConsoleHistoryWindowColour(CConsole& owner, const CGenericVarVector& params)
{
    owner.HistoryWindowColour = CRGBColour((unsigned char)params[0].GetULONG(), (unsigned char)params[1].GetULONG(),
                          (unsigned char)params[2].GetULONG(), 0xFF);
}

// Retail 009EAAB0.
void FABLE_FASTCALL CConsole::SetConsoleClientWindowColour(CConsole& owner, const CGenericVarVector& params)
{
    owner.ClientWindowColour = CRGBColour((unsigned char)params[0].GetULONG(), (unsigned char)params[1].GetULONG(),
                          (unsigned char)params[2].GetULONG(), 0xFF);
}

// Retail 009EAAE0.
void FABLE_FASTCALL CConsole::SetConsoleHelpWindowColour(CConsole& owner, const CGenericVarVector& params)
{
    owner.HelpWindowColour = CRGBColour((unsigned char)params[0].GetULONG(), (unsigned char)params[1].GetULONG(),
                          (unsigned char)params[2].GetULONG(), 0xFF);
}

// Retail 009EAB10.
void FABLE_FASTCALL CConsole::SetConsoleScrollBarColour(CConsole& owner, const CGenericVarVector& params)
{
    owner.ScrollBarColour = CRGBColour((unsigned char)params[0].GetULONG(), (unsigned char)params[1].GetULONG(),
                          (unsigned char)params[2].GetULONG(), 0xFF);
}
